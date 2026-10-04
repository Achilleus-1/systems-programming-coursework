import os
import pathlib
import shutil
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]

@unittest.skipUnless(os.name == 'posix', 'POSIX processes are validated by Linux CI')
class ProcessControlTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory = tempfile.TemporaryDirectory()
        cls.program = pathlib.Path(cls.directory.name) / 'commands'
        subprocess.run([shutil.which('gcc'), '-std=c11', '-Wall', '-Wextra',
                        '-Werror', str(ROOT / 'process-control/assign6.c'),
                        '-o', str(cls.program)], check=True)

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def run_commands(self, text):
        return subprocess.run([str(self.program), text], capture_output=True,
                              text=True, timeout=5)

    def test_each_child_gets_its_own_command(self):
        result = self.run_commands('echo first, echo second, echo third')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(set(result.stdout.splitlines()), {'first', 'second', 'third'})

    def test_many_arguments_fit_without_overflow(self):
        words = ['item' + str(i) for i in range(40)]
        result = self.run_commands('echo ' + ' '.join(words))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.strip().split(), words)

    def test_bad_input_and_child_failure_are_reported(self):
        for command in ('', '   ', 'echo x,' * 7, 'echo ' + 'x' * 1024,
                        'coursework-command-that-does-not-exist'):
            with self.subTest(command=command[:20]):
                self.assertNotEqual(self.run_commands(command).returncode, 0)

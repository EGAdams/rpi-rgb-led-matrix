# Persona
- World-class Python developer

# Goal
- Fix the argument problem in this code:
```python
class CommandRunner:
    def __init__(self, command, args=[]):
        self.command = command
        self.args = args

    def run(self):
        try:
            print( "running command: " + self.command + " " + str(self.args))
            output = subprocess.check_output([self.command] + self.args, stderr=subprocess.STDOUT, text=True)
            return output
        except subprocess.CalledProcessError as e:
            return e.output
```

# Background
The command that I am using is:
```bash
g++ -o ./SourceData/SourceDataTest ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl
running command: g++ -o ./SourceData/SourceDataTest ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl []
```

# Output
```bash
Exception has occurred: FileNotFoundError
[Errno 2] No such file or directory: 'g++ -o ./SourceData/SourceDataTest ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl'
  File "/home/adamsl/rpi-rgb-led-matrix/tennis-game/build_fix_make_prompt.py", line 16, in run
    output = subprocess.check_output([self.command] + self.args, stderr=subprocess.STDOUT, text=True)
  File "/home/adamsl/rpi-rgb-led-matrix/tennis-game/build_fix_make_prompt.py", line 35, in <module>
    command_output = command_to_run.run()
FileNotFoundError: [Errno 2] No such file or directory: 'g++ -o ./SourceData/SourceDataTest ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl'
```

I don't think my arguments are being passed correctly.

This is the string being passed in by the user.  The user is passing the command in exactly as it is written to work in the terminal.
```bash
 'g++ -o ./SourceData/SourceDataTest ./SourceData/SourceDataTest.cpp ./SourceData/SourceData.cpp ./FetchRunner/FetchRunner.cpp -lcurl'
```
Please write a Python method to change the string used on the command line to something that the CommandRunner object can use.

I have modified the CommandRunner object.  Does it look right to you?
```python
class CommandRunner:
    def __init__(self, command, args=[]):
        self.command = command.split()  # Split the command into a list
        self.args = args
    
    def parse_command( command_string ):
        parts = command_string.split()  # Split the command string into a list of words
        command = parts[ 0 ]              # The first part is the command
        args = parts[1:]                # The rest are the arguments
        return command, args

    def run(self):
        try:
            command_list = self.command + self.args  # Combine command and arguments
            print( "running command:", " ".join( command_list ))
            output = subprocess.check_output( command_list, stderr=subprocess.STDOUT, text=True )
            return output
        except subprocess.CalledProcessError as e:
            return e.output
```

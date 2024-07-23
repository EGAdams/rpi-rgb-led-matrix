import pexpect
import os

class OutputCatcher:
    def __init__(self, directory):
        self.directory = directory

    def capture_output(self, command):
        try:
            # Change directory to the specified directory
            os.chdir(self.directory)
            # Spawn the command using pexpect
            child = pexpect.spawn(command, encoding='utf-8', timeout=None)
            output = ""
            while True:
                try:
                    child.expect('\r\n', timeout=0.5)
                    output += child.before + "\n"
                except pexpect.exceptions.TIMEOUT:
                    output += child.before
                    break
                except pexpect.exceptions.EOF:
                    output += child.before



































                    break
            return output
        except Exception as e:
            return f"Error executing command: {str(e)}"

# Example usage:
if __name__ == "__main__":
    catcher = OutputCatcher("/home/adamsl/rpi-rgb-led-matrix/tennis-game")
    output = catcher.capture_output("make")
    print("Captured Output:\n", output)

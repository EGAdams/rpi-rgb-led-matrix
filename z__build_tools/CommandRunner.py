import subprocess

class CommandRunner:
    def __init__(self, command, args=[]):
        self.command = command
        self.args = args

    def run(self):
        try:
            output = subprocess.check_output([self.command] + self.args, stderr=subprocess.STDOUT, text=True)
            return output
        except subprocess.CalledProcessError as e:
            return e.output

# Usage example
# if this is main
if (__name__ == "__main__"):
    make_runner = CommandRunner("make")
    make_output = make_runner.run()
    print("Make command output:")
    print(make_output)

    # gpp_runner = CommandRunner("g++", ["-o", "myprogram", "myprogram.cpp"])
    # gpp_output = gpp_runner.run()
    # print("g++ command output:")
    # print(gpp_output)

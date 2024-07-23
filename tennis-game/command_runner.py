import pexpect

class CommandRunner:
    def run_command(self, command):
        try:
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

# Test CommandRunner with a command that will fail
if __name__ == "__main__":
    cr = CommandRunner()
    output = cr.run_command("make")
    # print the output in yellow
    print ("\033[93m" + output + "\033[0m")
    # print Done in green
    print ("\033[92m" + "Done" + "\033[0m")

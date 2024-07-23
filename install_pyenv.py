import pexpect
import sys
import os

def install_pyenv():
    # Install pyenv if it's not already installed
    if not os.path.exists(os.path.expanduser("~/.pyenv")):
        print("Installing pyenv...")
        child = pexpect.spawn('curl https://pyenv.run | bash')
        child.expect(pexpect.EOF)
        print(child.before.decode())

        # Add pyenv to shell configuration
        shell_config = os.path.expanduser("~/.bashrc")
        with open(shell_config, 'a') as f:
            f.write('\n# Pyenv configuration\n')
            f.write('export PATH="$HOME/.pyenv/bin:$PATH"\n')
            f.write('eval "$(pyenv init --path)"\n')
            f.write('eval "$(pyenv init -)"\n')
            f.write('eval "$(pyenv virtualenv-init -)"\n')

        print("Pyenv installed. Please restart your shell and run the second script to create a virtual environment.")
    else:
        print("pyenv is already installed")

if __name__ == "__main__":
    install_pyenv()

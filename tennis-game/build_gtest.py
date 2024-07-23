import os
import pexpect
import sys

def run_command(command, timeout=300):
    process = pexpect.spawn(command, timeout=timeout)
    process.logfile = sys.stdout.buffer  # Using buffer to handle bytes directly
    process.expect(pexpect.EOF)
    process.close()
    if process.exitstatus != 0:
        raise Exception(f"Command failed: {command}")

def install_googletest():
    print("Cloning GoogleTest repository...")
    run_command("git clone https://github.com/google/googletest.git")

    os.chdir('googletest')  # Change to the googletest directory

    print("Creating build directory...")
    os.mkdir('build')

    os.chdir('build')  # Change to the build directory

    print("Running CMake...")
    run_command("cmake ..")

    print("Building GoogleTest...")
    run_command("make")

    print("Installing GoogleTest...")
    run_command("sudo make install")

    print("GoogleTest installation complete!")

if __name__ == "__main__":
    try:
        install_googletest()
    except Exception as e:
        print(f"An error occurred: {e}")


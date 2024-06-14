import subprocess
import pexpect
import re

def get_removable_devices():
    try:
        # Run the lsblk command to get detailed information about all block devices
        result = subprocess.run(['lsblk', '-o', 'NAME,RM,SIZE,RO,FSTYPE,MOUNTPOINT'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        # Check if the command was successful
        if result.returncode != 0:
            print(f"Error running lsblk: {result.stderr}")
            return None

        lsblk_output = result.stdout
        print(f"lsblk output:\n{lsblk_output}")  # Debugging output

        # Extract information about removable devices
        removable_devices = []
        for line in lsblk_output.splitlines():
            print(f"Processing line: {line}")  # Debugging output
            if re.search(r'\b1\b', line):  # Look for the "RM" (removable) column with value 1
                removable_devices.append(line)

        return removable_devices
    except Exception as e:
        print(f"An error occurred: {e}")

    return None

def unmount_drive(mount_point, sudo_password):
    if not mount_point:
        print("No USB drive found.")
        return

    try:
        # Spawn a bash shell
        child = pexpect.spawn(f'sudo umount {mount_point}')

        # Expect the password prompt and send the sudo password
        i = child.expect([pexpect.TIMEOUT, 'password for'], timeout=10)
        if i == 0:  # Timeout or unexpected output
            print(f"Error: unexpected output or timeout when unmounting {mount_point}")
            return

        child.sendline(sudo_password)

        # Wait for the command to complete
        child.expect(pexpect.EOF)
        print(f"Drive unmounted from {mount_point}")
    except pexpect.exceptions.TIMEOUT as e:
        print(f"Timeout occurred during unmounting: {e}")
    except Exception as e:
        print(f"An error occurred during unmounting: {e}")

if __name__ == "__main__":
    sudo_password = "sep02@Th"
    removable_devices = get_removable_devices()
    if removable_devices:
        for device in removable_devices:
            print(f"Removable device found: {device}")
            mount_point_match = re.search(r'/\S*$', device)  # Find the mount point in the device line
            if mount_point_match:
                mount_point = mount_point_match.group(0)
                print(f"Mount point found: {mount_point}")
                unmount_drive(mount_point, sudo_password)
            else:
                print("No mount point found for this device.")
    else:
        print("Failed to find any removable devices.")

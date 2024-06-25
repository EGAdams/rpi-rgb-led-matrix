import subprocess
import sys
import os
from time import sleep
import argparse
from openai import OpenAI
the_makefile_directory = "/home/eg1972/rpi-rgb-led-matrix/tennis-game/TieBreaker"

# import CommandRunner from path: /home/eg1972/rpi-rgb-led-matrix/tennis-game/TieBreader/command_runner/command_runner.

# change import path to /home/eg1972/rpi-rgb-led-matrix/tennis-game/TieBreader/command_runner/command_runner.py
from command_runner.command_runner import CommandRunner

def read_file(filename):
    with open(filename, "r") as file:
        return file.read()


def walk_directory(directory):
    image_extensions = [
        ".png",
        ".jpg",
        ".jpeg",
        ".gif",
        ".bmp",
        ".svg",
        ".ico",
        ".tif",
        ".tiff",
        ".txt",
        ".js",
        ".editorconfig",
        ".md",
        ".py",
        ".pyc"
    ]

    code_contents = {}
    for root, dirs, files in os.walk(directory):
        for file in files:
            if not file.endswith((".h", ".cpp", ".cxx", ".c")):
                continue

            if not any(file.endswith(ext) for ext in image_extensions):
                try:
                    relative_filepath = os.path.relpath(
                        os.path.join(root, file), directory
                    )
                    code_contents[relative_filepath] = read_file(
                        os.path.join(root, file)
                    )
                except Exception as e:
                    code_contents[
                        relative_filepath
                    ] = f"Error reading file {file}: {str(e)}"
    return code_contents

def generate_response(system_prompt, user_prompt, model="gpt-3.5-turbo-0125", *args):
    # import openai

    # Set up your OpenAI API credentials
    # openai.api_key = os.environ["OPENAI_API_KEY"]
    # openai.api_key = "sk-"

    messages = []
    messages.append({"role": "system", "content": system_prompt})
    messages.append({"role": "user", "content": user_prompt})
    # loop thru each arg and add it to messages alternating role between "assistant" and "user"
    role = "assistant"
    for value in args:
        messages.append({"role": role, "content": value})
        role = "user" if role == "assistant" else "assistant"

    params = {
        # "model": model,
        "model": "gpt-3.5-turbo-0125",
        "messages": messages,
        "max_tokens": 4096,
        "temperature": 0,
    }

    # Send the API request
    keep_trying = True
    while keep_trying:
        try:
            client = OpenAI()
            response = client.chat.completions.create(**params)
            keep_trying = False
        except Exception as e:
            # e.g. when the API is too busy, we don't want to fail everything
            print("Failed to generate response. Error: ", e)
            sleep(30)
            print("Retrying...")

    # Get the reply from the API response
    reply = response.choices[0]["message"]["content"].strip()
    return reply

def main( args ):
    print ( "reading make file..." )
    # change directory to /home/eg1972/rpi-rgb-led-matrix/tennis-game/TieBreader
    os.chdir( the_makefile_directory )
    makefile = read_file("./Makefile")  # read Makefile from current directory

    prompt = """ \n# Make Output\n```bash\n"""
    cr = CommandRunner()
    make_output = cr.run_command( "make" )

    # put make_output into and array of strings
    make_output_array = make_output.splitlines()
    number_of_output_lines = len( make_output_array )
    print ( "Number of lines in make_output_array: ", number_of_output_lines )
    if ( number_of_output_lines > 100 ):
        print ( "output of command is greater than 100 lines. Truncating..." )
        # add the first 50 lines of output to prompt
        prompt += "\n".join( make_output_array[0:50] )
        prompt += "\n... truncated output...\n"
        # add the last 50 lines of output to prompt
        prompt += "\n".join( make_output_array[number_of_output_lines - 50:number_of_output_lines ])
    else:
        prompt += make_output

    # add ``` to the end of the prompt
    prompt += "```"

    # directory = args.directory
    model = "gpt-3.5-turbo-0125"
    # code_contents = walk_directory(directory)
    code_contents = walk_directory("/home/eg1972/rpi-rgb-led-matrix/tennis-game/TieBreaker")
    # code_contents = walk_directory("GameState")

    # Now, `code_contents` is a dictionary that contains the content of all your non-image files

    context = "\n".join(
        f"{path}:\n{contents}" for path, contents in code_contents.items()
    )
    system = "You are an AI debugger who is trying to debug a make error for a user based on their C++ source files and the Makefile used to build the project. The user has provided you with the following files and their contents, finally followed by the output of the make command:\n"
    prompt = (
        "# Source Files\n```cpp\n"
        + context + "```"
        + "\n\n# Makefile Source\n```bash\n"
        + makefile
        + "\n```\n"
        + prompt
    )
    prompt += (
        "\n\nPlease help me debug this."
    )
    print("number of lines in prompt: " + str(len(prompt.split("\n"))))

    # open a new file called prompt.md to start writing the final output to.
    with open(  "prompt.md", "w" ) as f:
        f.write( system )
        f.write( prompt )

    # print system message in purple
    print("\033[95m" + system + "\033[0m")
    # print prompt in yellow
    print("\033[96m" + prompt + "\033[0m")
    if input("press <enter> to continue or q to quit") == "q":
        sys.exit()
    res = generate_response(system, prompt, model)
    # print res in teal
    print("\033[96m" + res + "\033[0m")


# if main, run
if __name__ == "__main__":
    main( sys.argv )

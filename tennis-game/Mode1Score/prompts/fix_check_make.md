# Your Role
Expert Python Debugger

# Source Code to Analyze
```python
import subprocess
import sys
import os
from time import sleep
# from constants import DEFAULT_DIR, DEFAULT_MODEL, DEFAULT_MAX_TOKENS, EXTENSION_TO_SKIP
import argparse

from openai import OpenAI
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
    ]



    code_contents = {}
    for root, dirs, files in os.walk(directory):
        for file in files:
            # ask user if extension should be skipped
            answer = input(
                f"Should {file} be included in the prompt? (y/n) " )
            if answer == "n":
                # add just the extention to image_extensions array, not the file name.  we need to extract for example the ".png" from "file.png"
                image_extensions.append( file )
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


def main(args):
    makefile = read_file("/home/adamsl/rpi-rgb-led-matrix/tennis-game/Makefile") # read Makefile from current directory
    # change directory to /home/adamsl/rpi-rgb-led-matrix/tennis-game/Mode1Score
    os.chdir( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/Mode1Score" )
    # prompt=args.prompt
    prompt= """ Please fix this make error ``` """
    # run make command and add the output to the prompt. Make sure to consider that the subprocess is going to indeed fail, so we need to capture the failure and add it to the prompt without raising an exception and risk killing THIS process.
    prompt += "make output: " + str(subprocess.check_output(["make"], stderr=subprocess.STDOUT))

    # add ``` to the end of the prompt
    prompt += "```"

    directory= args.directory
    model=args.model
    # code_contents = walk_directory(directory)
    code_contents = walk_directory( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/Mode1Score" )
    # code_contents = walk_directory( "GameState" )

    # Now, `code_contents` is a dictionary that contains the content of all your non-image files


    context = "\n".join(
        f"{path}:\n{contents}" for path, contents in code_contents.items()
    )
    system = "You are an AI debugger who is trying to debug a make error for a user based on thier C++ source files and the Makefile used to build the project.  The user has provided you with the following files and their contents, finally folllowed by the output of the make command:\n"
    prompt = (
        "My files are as follows: "
        + context
        + "\n\n"
        + "Makefile: "
        + makefile
        + "\n\n"
        + "My issue is as follows: "
        + prompt
    )
    prompt += (
        "\n\nGive me ideas for what could be wrong and what fixes to do in which files."
    )
    print( "number of lines in prompt: " + str(len(prompt.split("\n"))))

    # print prompt in yellow
    print("\033[96m" + prompt + "\033[0m")
    if ( input( "press <enter> to continue or q to quit" ) == "q" ):
        sys.exit()
    res = generate_response(system, prompt, model)
    # print res in teal
    print("\033[96m" + res + "\033[0m")


def generate_response(system_prompt, user_prompt, model="gpt-3.5-turbo-0125", *args):
    import openai

    # Set up your OpenAI API credentials
    # openai.api_key = os.environ["OPENAI_API_KEY"]
    openai.api_key = "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"

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
        "max_tokens": 8000,
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


if __name__ == "__main__":
    DEFAULT_DIR = "."
    DEFAULT_MODEL = "gpt-3.5-turbo-0125"
    parser = argparse.ArgumentParser()
    # parser.add_argument(
    #     "prompt",
    #     help="The prompt to use for the AI. This should be the error message or issue you are facing.",

    # )
    parser.add_argument(
        "--directory",
        "-d",
        help="The directory to use for the AI. This should be the directory containing the files you want to debug.",
        default=DEFAULT_DIR,
    )
    parser.add_argument(
        "--model",
        "-m",
        help="The model to use for the AI. This should be the model ID of the model you want to use.",
        default=DEFAULT_MODEL,
    )
    args = parser.parse_args()
    main(args)
```

# Tail end of Error
```bash
...
make output. lines omitted for brevity
...

lISt6vectorINS0_9TraceInfoESaIS3_EEE11ValueHolderD5Ev]+0x25): undefined reference to `operator delete(void*, unsigned long)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/build/lib/libgtest.a(gtest-all.cc.o): in function `testing::internal::ThreadLocal<testing::TestPartResultReporterInterface*>::ValueHolder::~ValueHolder()':
gtest-all.cc:(.text._ZN7testing8internal11ThreadLocalIPNS_31TestPartResultReporterInterfaceEE11ValueHolderD0Ev[_ZN7testing8internal11ThreadLocalIPNS_31TestPartResultReporterInterfaceEE11ValueHolderD5Ev]+0x25): undefined reference to `operator delete(void*, unsigned long)'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/build/lib/libgtest.a(gtest-all.cc.o):(.data.rel.local.DW.ref._ZTISt9exception[DW.ref._ZTISt9exception]+0x0): undefined reference to `typeinfo for std::exception'
/usr/bin/ld: /home/adamsl/rpi-rgb-led-matrix/tennis-game/googletest/build/lib/libgtest.a(gtest-all.cc.o):(.data.rel.local.DW.ref.__gxx_personality_v0[DW.ref.__gxx_personality_v0]+0x0): undefined reference to `__gxx_personality_v0'
/usr/bin/ld: warning: creating DT_TEXTREL in a PIE
collect2: error: ld returned 1 exit status
make: *** [<builtin>: Mode1ScoreTest] Error 1
Traceback (most recent call last):
  File "/home/adamsl/rpi-rgb-led-matrix/tennis-game/check_make.py", line 163, in <module>
    main(args)
  File "/home/adamsl/rpi-rgb-led-matrix/tennis-game/check_make.py", line 60, in main
    prompt += "make output: " + str(subprocess.check_output(["make"]))
  File "/usr/lib/python3.9/subprocess.py", line 424, in check_output
    return run(*popenargs, stdout=PIPE, timeout=timeout, check=True,
  File "/usr/lib/python3.9/subprocess.py", line 528, in run
    raise CalledProcessError(retcode, process.args,
subprocess.CalledProcessError: Command '['make']' returned non-zero exit status 2.
(agent_99_environment) adamsl@penguin:~/rpi-rgb-led-matrix/tennis-game$
```

# Your Task
Please fix this error, the make command is expected to fail so that the output can be analyzed later.  Instead of having the make failure crash this script, I want the output of the fail to be captured.

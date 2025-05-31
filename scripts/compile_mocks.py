import os
import re
import subprocess

PYTHON_CMD = "python3" if os.name == "posix" else "python"
VENV_SCRIPT = r"^activate$" if os.name == "posix" else "activate.bat"


def find_files(directory, pattern):
    regex = re.compile(pattern)
    for root, dirs, files in os.walk(directory):
        for file in files:
            if regex.match(file):
                yield os.path.join(root, file)


def main(project_directory, output_directory, filepaths):
    pattern = r"^#include\s*\"mock_(.+)\"$"
    generator_path = ""
    venv_script_path = ""
    for script in find_files(project_directory, r"gmock_gen.py"):
        generator_path = script
    for script in find_files(project_directory, VENV_SCRIPT):
        venv_script_path = script
        venv_script_path = (
            ". " + venv_script_path if os.name == "posix" else venv_script_path
        )
    for filepath in filepaths:
        with open(filepath, "r") as file:
            for line in file:
                match = re.search(pattern, line)
                if match:
                    filename = match.group(1)
                    for found_file in find_files(project_directory, filename):
                        mock_file = os.path.join(output_directory, f"mock_{filename}")
                        if not os.path.exists(mock_file):
                            if generator_path != "" and venv_script_path != "":
                                if os.name == "posix":
                                    subprocess.run(
                                        [
                                            venv_script_path
                                            + "; "
                                            + PYTHON_CMD
                                            + " "
                                            + generator_path
                                            + " "
                                            + found_file
                                            + " "
                                            + output_directory
                                            + ";"
                                        ],
                                        shell=True,
                                    )
                                else:
                                    subprocess.run([venv_script_path], shell=True)
                                    subprocess.run(
                                        [
                                            PYTHON_CMD,
                                            generator_path,
                                            found_file,
                                            output_directory,
                                        ],
                                        shell=True,
                                    )
                            else:
                                raise FileNotFoundError(
                                    "GMock Generator Script not Found!"
                                )


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 4:
        print(
            "Usage: python compile_mocks.py <project_directory> <output_directory> <filepaths>"
        )
        print("Given Args:")
        for arg in sys.argv:
            print(arg)
        sys.exit(1)

    project_directory = sys.argv[1]
    output_directory = sys.argv[2]
    filepaths = sys.argv[3].split(";")

    try:
        main(project_directory, output_directory, filepaths)
    except Exception as e:
        print(f"An error occurred: {e}")

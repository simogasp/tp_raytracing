#!/usr/bin/env python3

"""
Script used to generate the package to be given to the student.
Things to adapt:
* default name for the package
* files to copy
* directories to copy
* name of the main files to studentify
"""

import argparse
import logging
import os
import shutil
import subprocess
import sys
import tempfile
import zipfile

logging.basicConfig(
    level=logging.INFO,
    format="[%(levelname)s] %(message)s"
)
logger = logging.getLogger()

DEFAULT_PREFIX = "tp_raytracing-v"


def generate_markers(comment_marker: str = "//", with_space: bool = False) -> list:
    """
    Generate a list of special markers for code generation.

    This function creates markers by combining a comment marker with special
    symbols (!!, ++, ??, ::) in three variations: plain, left-pointing (<),
    and right-pointing (>).

    Args:
        comment_marker (str): The comment marker to use (e.g., "//", "#").
                             Defaults to "//".
        with_space (bool): If True, adds a space between the comment marker
                          and the special symbols. Defaults to False.

    Returns:
        list: A list of generated marker strings.

    Example:
        >>> generate_markers("//", with_space=False)
        ['//!!', '//<!!', '//>!!', '//++', '//<++', ...
        >>> generate_markers("//", with_space=True)
        ['// !!', '// <!!', '// >!!', '// ++', ...
    """
    symbols = ["!!", "++", "??", "::"]
    markers = []
    separator = " " if with_space else ""

    for symbol in symbols:
        # Plain version: //!!
        markers.append(f"{comment_marker}{separator}{symbol}")
        # Left-pointing version: //<!!
        markers.append(f"{comment_marker}{separator}<{symbol}")
        # Right-pointing version: //>!!
        markers.append(f"{comment_marker}{separator}>{symbol}")

    return markers


# Special markers without spaces (compact format)
STUDENT_TOKENS = generate_markers(comment_marker="//", with_space=False)

# Special markers with spaces between comment tag and symbols
BAD_STUDENT_TOKENS = generate_markers(comment_marker="//", with_space=True)


def contains_special_markers(filename: str, markers: list) -> bool:
    """
    Check if a file contains any special markers used for code generation.

    This function scans the specified file for the presence of special
    comment markers that are typically used to indicate sections that need
    to be modified, removed, or replaced during code generation processes
    (e.g., when generating student versions of code).

    Args:
        filename (str): The path to the file to check. Can be an absolute
                       or relative path.
        markers (list): A list of marker strings to search for.

    Returns:
        bool: True if any of the special markers is found in the file,
              False otherwise.

    Raises:
        FileNotFoundError: If the specified file does not exist.
        IOError: If there is an error reading the file.

    Example:
        >>> contains_special_markers("src/MyClass.java")
        True
        >>> contains_special_markers("README.md", markers=STUDENT_TOKENS)
        False
        >>> contains_special_markers("code.java", markers=["//!!", "// ++"])
        True
    """
    try:
        with open(filename, 'r', encoding='utf-8') as file:
            content = file.read()
            for marker in markers:
                if marker in content:
                    # log the first occurrence of the found marker with the line number
                    line_number = content.count('\n', 0, content.find(marker)) + 1
                    logger.info(f"Found marker '{marker}' in file '{filename}' at line {line_number}")
                    return True
        return False
    except FileNotFoundError:
        logger.error(f"File not found: {filename}")
        raise
    except IOError as e:
        logger.error(f"Error reading file {filename}: {e}")
        raise


def are_files_identical(file1: str, file2: str) -> bool:
    """
    Check if two text files have exactly the same content.

    This function reads two text files and compares their contents
    character-by-character to determine if they are identical. The
    comparison is case-sensitive and includes all whitespace and
    formatting.

    Args:
        file1 (str): The path to the first file to compare.
        file2 (str): The path to the second file to compare.

    Returns:
        bool: True if both files have exactly the same content,
              False otherwise.

    Raises:
        FileNotFoundError: If either file does not exist.
        IOError: If there is an error reading either file.

    Example:
        >>> are_files_identical("file1.txt", "file2.txt")
        True
        >>> are_files_identical("original.java", "modified.java")
        False
    """
    try:
        with open(file1, 'r', encoding='utf-8') as f1:
            content1 = f1.read()
        with open(file2, 'r', encoding='utf-8') as f2:
            content2 = f2.read()
        return content1 == content2
    except FileNotFoundError as e:
        logger.error(f"File not found: {e.filename}")
        raise
    except IOError as e:
        logger.error(f"Error reading files: {e}")
        raise


def clean_workspace(directory: str) -> None:
    """
    Remove a directory and all its contents if it exists.

    This function checks if a directory exists at the given path and removes
    it along with all its contents (files and subdirectories). If the directory
    does not exist, the function does nothing.

    Args:
        directory (str): The path to the directory to remove. Can be an
                        absolute or relative path.

    Returns:
        None

    Raises:
        OSError: If there is an error removing the directory (e.g., permission
                denied, directory in use).

    Example:
        >>> clean_workspace("/tmp/my_temp_folder")
        >>> clean_workspace("build/output")
    """
    if os.path.exists(directory):
        try:
            logger.info(f"Removing directory: {directory}")
            shutil.rmtree(directory)
            logger.info(f"Successfully removed directory: {directory}")
        except OSError as e:
            logger.error(f"Error removing directory {directory}: {e}")
            raise
    else:
        logger.debug(f"Directory does not exist, nothing to remove: {directory}")


def verify_no_markers_in_directory(directory: str, markers: list, studentify_dir: str) -> None:
    """
    Verify that all Java files in a directory do not contain special markers.

    This function recursively walks through the specified directory and checks
    all Java files to ensure they don't contain any of the special markers
    that should have been processed during studentification.

    Args:
        directory (str): The path to the directory to check.
        markers (list): A list of marker strings to search for.
        studentify_dir (str): The path to the studentify directory to clean up
                             in case of errors.

    Returns:
        None

    Raises:
        SystemExit: If any file contains special markers, exits with code 1
                   after cleaning up.

    Example:
        >>> verify_no_markers_in_directory("dist/src", STUDENT_TOKENS, "/tmp/tpt")
    """
    for root, _, files in os.walk(directory):
        for file in files:
            if not file.endswith(".java"):
                continue
            file_path = os.path.join(root, file)
            if contains_special_markers(file_path, markers):
                logger.error(f"File {file_path} still contains special markers after processing.")
                clean_workspace(studentify_dir)
                sys.exit(1)


def main(working_dir: str, studentify_dir: str, archive_name: str, skip_cleaning: bool = False, skip_packaging: bool = False):

    if not os.path.exists(working_dir):
        logger.info(f"Creating working directory {working_dir}")
        os.makedirs(working_dir)

    dest_name = os.path.join(working_dir, archive_name)

    logger.info(f"Creating directory {dest_name}")
    os.mkdir(dest_name)

    files_to_copy = [
        "README.md",
        "BUILD.md"
    ]
    logger.info(f"Copying files to {dest_name}")
    for file in files_to_copy:
        shutil.copy(file, dest_name)

    logger.info(f"Copying directories in {dest_name}")
    # shutil.copytree(".vscode", os.path.join(dest_name, ".vscode"))
    shutil.copytree("src", os.path.join(dest_name, "src"))

    # Define trusted repository URL as a constant
    STUDENTIFY_REPO = "https://github.com/simogasp/studentipy.git"

    logger.info(f"Cloning studentify.py to {studentify_dir}")
    # Use absolute path to git command and validate the directory path
    studentify_dir_abs = os.path.abspath(studentify_dir)
    subprocess.check_call([
        "/usr/bin/git",
        "clone",
        STUDENTIFY_REPO,
        studentify_dir_abs
    ])

    files_to_studentify = ["src/raytracing/controller/Renderer.cpp",
                           "src/raytracing/core/hittableObject/Sphere.cpp",
                          "src/raytracing/core/Camera.cpp"]
    for file in files_to_studentify:
        file_path = os.path.join(dest_name, file)
        logger.info(f"Applying studentify to {file_path}")
        subprocess.check_call(
            ["python3", os.path.join(studentify_dir, "studentify.py"), file_path, "-o", file_path, "--force"])
        # check that the file does not contain any special markers
        if contains_special_markers(file_path, STUDENT_TOKENS + BAD_STUDENT_TOKENS):
            logger.error(f"File {file_path} still contains special markers after applying studentify.")
            clean_workspace(studentify_dir)
            sys.exit(1)
        # check that the file is not identical to the original
        original_file_path = file
        if are_files_identical(original_file_path, file_path):
            logger.error(f"File {file_path} is identical to the original after applying studentify.")
            clean_workspace(studentify_dir)
            sys.exit(1)

    # # remove Depth and Normal shader implementations
    # files_to_remove = [
    #     "src/renderer/core/shader/DepthShader.java",
    #     "src/renderer/core/shader/NormalMapShader.java",
    #     "test/unit/core/shader/DepthBufferTest.java"
    # ]
    # for file in files_to_remove:
    #     file_path = os.path.join(dest_name, file)
    #     logger.info(f"Removing {file_path}")
    #     os.remove(file_path)

    # check that all the java files in src and test do not contain any special markers
    logger.info("Verifying that no special markers are left in the code")
    all_markers = STUDENT_TOKENS + BAD_STUDENT_TOKENS
    verify_no_markers_in_directory(os.path.join(dest_name, "src"), all_markers, studentify_dir)
    verify_no_markers_in_directory(os.path.join(dest_name, "test"), all_markers, studentify_dir)

    logger.info("Removing studentify.py")
    shutil.rmtree(studentify_dir)

    if not skip_packaging:
        logger.info(f"Generating archive {archive_name}.zip in {working_dir}")
        with zipfile.ZipFile(os.path.join(working_dir, f"{archive_name}.zip"), "w", compression=zipfile.ZIP_BZIP2) as zip_file:
            for root, _, files in os.walk(dest_name):
                for file in files:
                    zip_file.write(os.path.join(root, file), os.path.relpath(os.path.join(root, file), dest_name))

    if skip_cleaning:
        return
    logger.info(f"Cleaning up {dest_name}")
    shutil.rmtree(dest_name)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script used to generate the package to be given to the student.")
    parser.add_argument(
        "-v",
        "--verbosity",
        dest="verbosity",
        choices=["DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"],
        default="INFO",
        help="Set the logging verbosity level (choose from DEBUG, INFO, WARNING, ERROR, CRITICAL).",
    )
    parser.add_argument(
        "-w",
        "--working-dir",
        dest="working_dir",
        help="Set the working directory where the files will be created.",
    )
    parser.add_argument(
        "-a",
        "--archive-name",
        dest="archive_name",
        help=f"The name of the archive to be created. [default={DEFAULT_PREFIX}<version>]"
    )
    parser.add_argument(
        "--skip-cleaning",
        action='store_true',
        dest="skip_cleaning",
        help="Skip the cleaning step leaving the code available.",
    )
    parser.add_argument(
        "--skip-packaging",
        action='store_true',
        dest="skip_packaging",
        help="Skip the packaging step without generating the archive.",
    )

    args = parser.parse_args()

    if args.verbosity:
        logger.setLevel(args.verbosity)

    if args.working_dir:
        wdir = args.working_dir
    else:
        wdir = tempfile.gettempdir()

    if args.archive_name:
        zip_name = args.archive_name
    else:
        version = subprocess.check_output(["git", "describe", "--tags"]).decode("utf-8").strip()[1:]
        zip_name = f"{DEFAULT_PREFIX}{version}"

    studentify_dir = os.path.join(tempfile.gettempdir(), "tpt")
    try:
        main(working_dir=wdir, studentify_dir=studentify_dir, archive_name=zip_name, skip_cleaning=args.skip_cleaning, skip_packaging=args.skip_packaging)
    except Exception as e:
        logger.exception(e)
        clean_workspace(studentify_dir)
        sys.exit(1)

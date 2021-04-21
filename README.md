# Prebuilt modm libraries

This repository contains the already-generated modm library and documentation for software-dolken, so that users do not need to have lbuild and python installed on their computers. It is used as a submodule in the [software-dolken repository](https://github.com/DanSTAR-DTU/software-dolken).

## Issue pushing to branch
If you're using a ssh key, then use the following command
* git remote set-url origin git@github.com:DanSTAR-DTU/modm-prebuilt.git

## Updating

### Tools Required

* Python 3

* lbuild (available through pip)

* Doxygen

* Graphviz

All of these tools should have their commands in the system path. With the exception of lbuild, Linux users should be able to obtain them through their system package manager, while Windows users should be able to obtain them through the Scoop package manager or standalone installers.

### Procedure

0. Currently running a custom version of modm with modifications to usart1 and usart2. These changes should be manually added when updating modm.

1. Check out submodules of this repository using `git submodule update --init --recursive`

2. If these changes are experimental, switch to a branch other than `master`.

3. If upgrading to a newer version of modm, navigate to the `repo` directory and check out a newer commit for modm.

4. If adding/removing modules from build, edit `project.xml`.

5. Delete the already built folder using `rm -rf modm` or similar.

6. Run `lbuild build` to regenerate the modm files.

7. Update modm/repo.cmake in this repository if necessary to ensure any new directories with source and include files are compiled. The appending of flags might not work with our build and have to be modified by hand to `cache` the flags and use `set` instead of `append`.

8. Ensure the newly generated code compiles with the firmware and test on hardware as appropriate.

9. Commit and push all changes.

10. Update repositories using this one as a submodule to use the new commit.

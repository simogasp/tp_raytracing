# Deployment

To generate the code for the student, run the following command:

```bash
./generateStudent.py -w dist
```

This will generate a `dist` folder with the code for the student and inside you will find a zip file with the code named `tp_raytracing-vX-Y-Z.zip`.
The version `X.Y` is the most recent tag in the git repository and `Z` is the commit hash (this is added only if the most recent tag is not the current commit).

The python script has some option (check the help with `./generateStudent.py -h`):

- `-w` or `--workdir` to specify the working directory (default is a random folder in `/tmp`).
- `--skip-cleaning` will keep in the working directory the folder with all the files generated for the student, so you can check them more easily.
- `--skip-packaging` will skip the creation of the zip file.

> [!NOTE]
> The script needs to download a repository so you need to have the internet connection active.

You can check the documentation on how to use the comment tags to hide or replace the code [on the original repository](https://github.com/simogasp/studentipy/blob/master/README.md)

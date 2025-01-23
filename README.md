[![Automatic version updates](https://github.com/ZOSOpenTools/rsyncport/actions/workflows/bump.yml/badge.svg)](https://github.com/ZOSOpenTools/rsyncport/actions/workflows/bump.yml)

# rsync

rsync requires a 'modern' awk, Python3 with commonmark module, and autotools
when building using 'git'.
See: (https://github.com/WayneD/rsync/blob/master/INSTALL.md)

# Caveats
* Files on z/OS are tagged as `ISO8859-1` by default during a sync, and file tags are not preserved.  
  Curently, the only way to change the file tag is by using the `_ENCODE_FILE_NEW` environment variable. For example:  
  ```bash
  export _ENCODE_FILE_NEW=IBM-1047
  ```
* Preserving file tags is a **TODO** item and is not yet implemented.
* Since rsync is not installed in the default PATH location on z/OS, it's recommended that you set the remote path by using the --rsync-path option. For example `--rsync-path=/home/itodoro/zopen/usr/local/bin/rsync`

# What Works

### Sync Files from One Local Directory to Another
```bash
rsync -avz /home/user/source/ /home/user/destination/
```

### Sync Entire z/OS UNIX Directory to Non-z/OS
```bash
rsync -avz /u/userid/mydir/ user@linuxhost:/home/user/mydir_backup/
```

### Sync Entire Non-z/OS Directory to z/OS
```bash
rsync -avz --rsync-path=<zos_rsync_path> /home/user/project/ user@zoshost:/u/userid/project/
```

### Using a Different SSH Port
```bash
rsync -avz -e "ssh -p 2222" /home/user/project/ user@zoshost:/u/userid/project/
```

### Delete Files on the Target That No Longer Exist in the Source
```bash
rsync -avz --delete /home/user/project/ user@linuxhost:/home/user/project_backup/
```

# What's Not Supported
* **Datasets**: Syncing datasets is not currently supported.
* **File Tag Preservation**:  
  Files on z/OS are tagged as `ISO8859-1` by default during a sync, and file tags are not preserved.  
  The only current way to affect the file tag is by using `_ENCODE_FILE_NEW`. For example:  
  ```bash
  export _ENCODE_FILE_NEW=IBM-1047
  ```
  Preserving file tags is a **TODO** item and is not yet implemented.

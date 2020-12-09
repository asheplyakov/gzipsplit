# Split concatenated gzip archives


## What?

Some Linux distributions (in particular [ALT Linux](http://www.altlinux.org))
use tricky initramfs images which consist of concatenated gzip archives.
This little program helps to split such initramfs images into individual
gzip files.

## Usage

```bash
$ gzipsplit /path/to/full.cz
  writing file #1
  writing file #2
  writing file #3
```

The output will be written to files `s_N.gz` (with N = 0, 1, 2 in
this example). This naming is hard coded, sorry.

## Building

```bash
$ make check
```

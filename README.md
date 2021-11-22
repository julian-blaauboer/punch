# Punch

Punch is a simple punch clock utility.

## Portability

Punch is aimed to be portable across Unix-like OSes. The provided scripts only
depend on a few common POSIX utilities and do not depend on any GNU-specific
extensions. The program itself should compile under any C89/C90 compatible
compiler.


## Building and installing

First, check [config.h](config.h) and make any changes as necessary. It is also
possible to use the [reconfigure](tools/reconfigure) script to edit a
`#define` inside [config.h](config.h), which can be useful for packaging:

```sh
tools/reconfigure PREFIX /usr/local
```

Compiling punch can be as simple as doing:

```sh
gcc -o punch punch.c
```

Although it is recommended that you use the provided [build](tools/build)
script:

```sh
tools/build
```

To install, you can use the [install](tools/install) script to install it on
your system:

```sh
tools/install
```

If you wish to install it to a different root directory (e.g., for packaging),
you can pass the directory as an argument to [install](tools/install). This
is equivalent to `DESTDIR` used by most Makefiles:

```sh
tools/install "$pkgdir"
```

It might be useful to create an archive of the source code for distribution.
For this, you can use [tools/dist](tools/dist):

```sh
tools/dist gzip
tools/dist xz
```

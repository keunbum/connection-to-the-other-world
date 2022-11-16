# Some Error Resolution Or Useful Tips in Programming, Git, Etc.

## GCC

### Compile

In ~/.bashrc just put like this.

```bash
# some my functions
c() {
    # compile $1.c to $1
    gcc "$1".c -o "$1"
}
```

By this, you don't need to put like this,
```bash
$ gcc my_foo_bar_baz.c -o my_foo_bar_baz
```

just
```bash
$ c my_foo_bar_baz
```

## Git

### git push

* [git wsl documentation bug git credential manager core.exe not found](https://github.com/microsoft/WSL/issues/8395#issuecomment-1121629434)
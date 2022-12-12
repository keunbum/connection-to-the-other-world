# Some Error Resolution Or Useful Tips in Windows.

## Index

* [Visual Studio](#visual-studio)
    * [template](#template)
* [CMD](#cmd)
    * [compile](#compile)
	* [background exec](#background-exec)
* [Programming](#programming)
    * [Data Types](#data-types)

## Visual Studio

### template

Use the following template code for network socket programming on Windows.

[ex_server_win.c](../../../template/ex_server_win.c)

[ex_client_win.c](../../../template/ex_client_win.c)


## CMD

### compile

for more [info](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax?view=msvc-170)

```cmd
> cl main.c
```

### background exec

```cmd
$ start .\main.exe
```

## Programming

### Data Types

���������� ```uint16_t```�� ```uint32_t```�� ���� ��ó��  
�����쿡���� ```u_short```�� ```u_int``` ���.


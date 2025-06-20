
* **bits/stat.h*
* dirent.h
* libgen.h
* sys/stat.h


 stat - 无法识别链接文件
lstat - 可以识别链接文件


```c
int scandir (const char *dirp, struct dirent ***namelist,
    typeof(int (const struct dirent *)) *filter,
    typeof(int (const struct dirent **, const struct dirent **)) *compar);
```

scandir扫描dirp目录，并对每一个条目执行filter函数, 如果filter返回一个非零值， 则该条目将存
储在malloc分配的namelist中， 并执行compar进行排序。


```c
int alphasort (const struct dirent **a, const struct dirent **b);
int versionsort (const struct dirent **a, const struct dirent **b);
```

```c
int scandirat (int dirfd, const char *dirp, struct dirent *** namelist,
    typeof(int (const struct dirent *)) *filter,
    typeof(int (const struct dirent **, const struct dirent **)) *compar);
```

scandirat类似于scandir，主要区别在于如果dirp为相对目录， 则会相对于dirfd的， 绝对目录无区别

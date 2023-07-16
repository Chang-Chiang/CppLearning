#!/bin/bash
# 删除无后缀文件
# grep -v 只允许不包含点的文件名
# xargs rm 将文件名列表传递给 rm
ls | grep -v "\." | xargs rm

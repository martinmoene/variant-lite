#!/usr/bin/env python
# -*- coding: utf-8 -*-

from jinja2 import Template, Environment, FileSystemLoader

def ProcessTemplate(src, dst):
    loader = FileSystemLoader('.')
    env = Environment(loader=loader)

    tpl = env.get_template(src)
    result = tpl.render(NumParams=16)
#    print (result)
    fout = open(dst, 'wt')
    fout.write(result)
    fout.close()

def main():
    ProcessTemplate('template/variant.hpp', 'include/nonstd/variant.hpp')

if __name__== "__main__":
  main()

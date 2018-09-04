#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import argparse

from jinja2 import Template, Environment, FileSystemLoader

def_variant_type_count = 16
def_visitor_args = 5

def ProcessTemplate(src, dst, cnt, va):
    """Edit variant header template to variant header supporting the specified maximum number of variant types."""

    loader = FileSystemLoader('.')
    env = Environment(loader=loader)

    tpl = env.get_template(src)
    result = tpl.render(NumParams=cnt, VisitorArgs=va)
#    print (result)
    fout = open(dst, 'wt')
    fout.write(result)
    fout.close()

def main():
    parser = argparse.ArgumentParser(
        description='Generate variant header from template header file.',
        epilog="""""",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='report the name of the file being processed')

    parser.add_argument(
        '--count',
        metavar='count',
        type=int,
        default=def_variant_type_count,
        help='number of variant types')

    parser.add_argument(
        '--visitor_args',
        metavar='visitor_args',
        type=int,
        default=def_visitor_args,
        help='number of arguments for \'visit\' methods')

    args = parser.parse_args()

    ProcessTemplate('template/variant.hpp', 'include/nonstd/variant.hpp', args.count, args.visitor_args)

if __name__== "__main__":
  main()

#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import argparse

from jinja2 import Template, Environment, FileSystemLoader

def_max_types = 16
def_max_args = 5

def ProcessTemplate(src, dst, types, args, verbose):
    """Edit variant header template to variant header supporting the specified 
    maximum number of variant types and of visitor arguments."""

    if verbose:
        print( "Generating '{hdr}' with max {tcnt} types, max {acnt} visitor arguments from '{tpl}':".format( tpl=src, hdr=dst, tcnt=types, acnt=args ) )
        
    loader = FileSystemLoader('.')
    env = Environment(loader=loader)

    tpl = env.get_template(src)
    result = tpl.render(NumParams=types, VisitorArgs=args)
#    print (result)
    fout = open(dst, 'wt')
    fout.write(result + '\n')
    fout.close()

def main():
    parser = argparse.ArgumentParser(
        description='Generate variant header from template header file.',
        epilog="""""",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='report what is generated')

    parser.add_argument(
        '--max-types',
        metavar='types',
        type=int,
        default=def_max_types,
        help='number of variant types')

    parser.add_argument(
        '--max-args',
        metavar='args',
        type=int,
        default=def_max_args,
        help='number of arguments for \'visit\' methods')

    opt = parser.parse_args()

    ProcessTemplate('template/variant.hpp', 'include/nonstd/variant.hpp', opt.max_types, opt.max_args, opt.verbose)

if __name__== "__main__":
  main()

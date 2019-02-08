#!/usr/bin/env python
#
# Copyright 2017-2018 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# script/upload-conan.py
#

from __future__ import print_function

import argparse
import os
import re
import sys
import subprocess

# Configuration:

def_conan_project = 'variant-lite'
def_conan_user    = 'nonstd-lite'
def_conan_channel = 'stable'
cfg_conanfile     = 'conanfile.py'

tpl_conan_create  = 'conan create . {usr}/{chn}'
tpl_conan_upload  = 'conan upload --remote {usr} {prj}/{ver}@{usr}/{chn}'

# End configuration.

def versionFrom( filename ):
    """Obtain version from  conanfile.py"""
    with file( filename ) as f:
        content = f.read()
        version = re.search(r'version\s=\s"(.*)"', content).group(1)
    return version

def createConanPackage( project, user, channel, version, verbose ):
    """Create conan package and upload it."""
    cmd = tpl_conan_create.format(usr=user, chn=channel)
    if verbose:
        print( "> {}".format(cmd) )
    subprocess.call( cmd, shell=False )
    
def uploadConanPackage( project, user, channel, version, verbose ):
    """Create conan package and upload it."""
    cmd = tpl_conan_upload.format(prj=project, usr=user, chn=channel, ver=version)
    if verbose:
        print( "> {}".format(cmd) )
    subprocess.call( cmd, shell=False )
    
def uploadToConan( project, user, channel, version, verbose ):
    """Create conan package and upload it."""
    print( "Updating project '{prj}' to user '{usr}', channel '{chn}', version {ver}:".
        format(prj=project, usr=user, chn=channel, ver=version) )
    createConanPackage( project, user, channel, version, verbose )
    uploadConanPackage( project, user, channel, version, verbose )
    
def uploadToConanFromCommandLine():
    """Collect arguments from the commandline and create conan package and upload it."""

    parser = argparse.ArgumentParser(
        description='Create conan package and upload it to conan.',
        epilog="""""",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-v', '--verbose',
        action='count',
        help='report upload')

    parser.add_argument(
        '--project',
        metavar='p',
        type=str,
        default=def_conan_project,
        help='conan project')

    parser.add_argument(
        '--user',
        metavar='u',
        type=str,
        default=def_conan_user,
        help='conan user')

    parser.add_argument(
        '--channel',
        metavar='c',
        type=str,
        default=def_conan_channel,
        help='conan channel')

    parser.add_argument(
        '--version',
        metavar='v',
        type=str,
        default=versionFrom( cfg_conanfile ),
        help='version number [from conanfile.py]')

    args = parser.parse_args()

    uploadToConan( args.project, args.user, args.channel, args.version, args.verbose )


if __name__ == '__main__':
    uploadToConanFromCommandLine()

# end of file

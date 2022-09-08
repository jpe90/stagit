#!/bin/bash

usage() { echo "Usage: $0 [-u <string>] [-d <string>]" 1>&2; exit 1; }
URLPREFIX="git://jeskin.net"
OWNER="Jon Eskin"

while getopts "d:" o; do
    case "${o}" in
        d)
            d=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

repodir="$1.git"
url="${URLPREFIX}/${repodir}"
git init --bare ${repodir}
cd ${repodir}
touch git-daemon-export-ok
echo ${d} > description
echo ${OWNER} > owner
echo ${url} > url

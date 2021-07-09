#!/bin/bash

if [ $# -lt 1 ]; then
     echo "usage: sh download_vscode_server.sh <commit_id>"
     exit 1
fi

# step 1:
commit_id=$1

# step 2: Download vs code server
wget --no-check-certificate "https://update.code.visualstudio.com/commit:${commit_id}/server-linux-x64/stable" -O /tmp/vscode-server-linux-x64.tar.gz

# step 3:
mkdir -p ~/.vscode-server/bin/${commit_id}

# step 4:
tar zxvf /tmp/vscode-server-linux-x64.tar.gz -C ~/.vscode-server/bin/${commit_id} --strip 1
rm -rf /tmp/vscode-server-linux-x64.tar.g

# step 5:
touch ~/.vscode-server/bin/${commit_id}/0


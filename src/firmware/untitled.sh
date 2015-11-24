#!/bin/sh


git clone --bare https://github.com/shivamndave/tiny_house.git
cd tiny_house.git





git filter-branch --env-filter '
OLD_EMAIL="Sargis@eduroam-169-233-251-253ucsc.edu"
CORRECT_NAME="Sargis Yonan"
CORRECT_EMAIL="sargisy@gmail.com"
if [ "$GIT_COMMITTER_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$CORRECT_NAME"
    export GIT_COMMITTER_EMAIL="$CORRECT_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$CORRECT_NAME"
    export GIT_AUTHOR_EMAIL="$CORRECT_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags
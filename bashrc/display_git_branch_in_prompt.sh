# This script exposes 3 features:
# ---
# 1. display git branch in prompt
# e.g.
# user@my-pc:scripts (master)
#                     ^^^^^^
# 2. allows user to choose between display full path on terminal vs last folder
# 3. set a color of your preference to the terminal user prompt + folder

# if true, only current folder is displayed on prompt
# e.g. true
# user@my-pc:personal_config
# user@my-pc:scripts
#            ^^^^^^^
# e.g. false
# user@my-pc:~/scripts
DISPLAY_LAST_FOLDER_IN_PATH=true

# extract branch name from repo
function git_branch 
{
    # get current branch (as text and without the *)
    git branch --no-color 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1) /'
}

# prompt color selection
# ---
#bash_color=32 # green color
# bash_color=31 # red color
# bash_color=33 # yellow color
# bash_color=34 # blue color
bash_color=35 # purple color
# bash_color=36 # cyan color

# check if personal config file for prompt color exists
FILE=$HOME/scripts/personal_config/prompt_color.sh
if [ -f "$FILE" ]; then
    # overrides "bash_color" variable with custom value
    source $HOME/scripts/personal_config/prompt_color.sh
fi

# modify prompt with custom configuration selected above
if [ "$DISPLAY_LAST_FOLDER_IN_PATH" = true ]; then
    # show only the last folder in the path
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;${bash_color}m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\W\[\033[00;32m\] $(git_branch)\[\033[00m\]\$ '
else
    # show the complete terminal path
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;${bash_color}m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00;32m\] $(git_branch)\[\033[00m\]\$ '
fi


# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/opt/anaconda3/bin/conda' 'shell.zsh' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/opt/anaconda3/etc/profile.d/conda.sh" ]; then
        . "/opt/anaconda3/etc/profile.d/conda.sh"
    else
        export PATH="/opt/anaconda3/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<

# start of AM129 .zshrc edits
export AM129_PATH="/Users/mratcliff/Documents/GitLab/ratcliff-max-am129-fall24"

alias wcl='wc -l'

function cd_up() {
cd $(printf "%0.0s../" $(seq 1 $1));
}
alias 'cd..'='cd_up'

function pwd_up() {
PREVDIR=$(pwd)
cd $(printf "%0.0s../" $(seq 1 $1)) && pwd && cd $PREVDIR; 
}
alias 'pwd..'='pwd_up'

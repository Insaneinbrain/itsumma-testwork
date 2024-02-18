# ITSumma bash-connector

# Manual start alias
alias itsumma-env="SUDO_USER=itsumma source /root/.itsumma/bashrc.inc"


# are we an interactive shell?

if [ "${SUDO_USER}" == "itsumma" ] && [ "${ITSUMMA_PS_SET}" != "1" ] && [ "$PS1" ]; then
    source ~/.itsumma/environment.inc;
    timeout -s SIGKILL 5s ~/.itsumma/bin/motd.sh || echo -e "Killed by timeout...\n\n"

    # Append to current $PS1
    # export PS1="\[\e[32m\][${ITSUMMA_PROJECT}]\[\e[m\]${PS1}";

    # ITSumma-env custom PS1
    export PS1="\[\e[32m\][${ITSUMMA_PROJECT}]\[\e[m\][\u@${ITSUMMA_HOST} \W]# ";

    # Set terminal title
    export PROMPT_COMMAND='echo -ne "\033]0;${ITSUMMA_PROJECT} | ${ITSUMMA_HOST}\007"'
fi

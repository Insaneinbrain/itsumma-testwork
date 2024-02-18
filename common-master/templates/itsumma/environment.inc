# ITSumma main environment variables
# {{ ansible_managed }}
# Git: {{ lookup('env','GIT_COMMIT') }} {{ ansible_date_time.iso8601 }}

# Define main variables
export PATH="~/.itsumma/bin:${PATH}"

# Holy Wars - Chapter 1
# export EDITOR="nano"

export ITSUMMA_PS_SET=1;
export ITSUMMA_PROJECT={{ project }}
export ITSUMMA_HOST={{ itsumma_host }}
export ITSUMMA_SERVER_UID=$(grep -E 'id [0-9]+$' /etc/essm.conf 2>/dev/null | grep -Eo '[0-9]+' || echo 'not found /etc/essm.conf')

# Reset to English locale
export LANG="en_US.UTF-8"
export LC_ADDRESS="$LANG"
export LC_IDENTIFICATION="$LANG"
export LC_MEASUREMENT="$LANG"
export LC_MONETARY="$LANG"
export LC_NAME="$LANG"
export LC_NUMERIC="$LANG"
export LC_PAPER="$LANG"
export LC_TELEPHONE="$LANG"
export LC_TIME="$LANG"
export LC_ALL="$LANG"

# Aliases
alias m="motd.sh"
alias mip="dig @208.67.222.222 myip.opendns.com +short"
alias php-locked-sessions="lsof -n | awk -f ~/.itsumma/sessions.awk"
alias dmesg-s="dmesg|tail -n50"

# Show accurate start time of process
pstime () {
    ps -p $1 -o lstart=
}

cgrep () {
    grep --color -HiRn ${1} /etc/essm*;
}

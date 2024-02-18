#!/bin/bash
# Our motd with useful info

# Our jenkins root access WARNING
ITSUMMA_TMP_KEYS=$(grep -Ei '(ops@jenkins|itsumma@ops-jenkins|itsumma)' ~/.ssh/authorized_keys 2>/dev/null | wc -l )

WAN_IP=$(dig @208.67.222.222 myip.opendns.com +short 2>/dev/null || curl -s -4 -s 'ifconfig.co' || echo 'none')
EXTERNAL_IP=$(ip  route get 8.8.8.8| awk 'FS=" " { print $7 }'|head -n1 )

echo -e '\n';

w -i

echo -e '';

echo "        Project:  ${ITSUMMA_PROJECT}";
echo "       Hostname:  ${ITSUMMA_HOST}";
echo "      Server id:  ${ITSUMMA_SERVER_UID}";
echo "          24mon:  https://${ITSUMMA_PROJECT}.24mon.com/#/${ITSUMMA_SERVER_UID}"
echo "";
echo "             OS:  {{ ansible_distribution }} {{ ansible_distribution_release }} {{ ansible_distribution_version }} ({{ ansible_machine }})";
echo "           Virt:  {{ ansible_virtualization_role }} ({{ ansible_virtualization_type }})";
echo "         WAN IP:  ${WAN_IP}";
echo " Default Ext IP:  ${EXTERNAL_IP}";


if grep ^md -c /proc/mdstat >/dev/null 2>&1; then
    echo "     Linux RAID:  $(~/.itsumma/bin/linux-raid.sh)";
fi

if [ "${ITSUMMA_TMP_KEYS}" != "0" ]; then
    echo  -e "\n\e[93m WARNING - Temporary keys found in ~/.ssh/authorized_keys\e[39m"
fi

echo -e '';
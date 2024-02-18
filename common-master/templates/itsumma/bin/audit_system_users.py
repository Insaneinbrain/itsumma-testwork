#!/usr/bin/env python

shell = {}
homes = {}
auth_keys = {}

with open('/etc/passwd', 'r') as file_object:

    for line in file_object:
        line = line.strip()
        if line == '':
            continue
        fields = line.split(":")
        shell[fields[0]] = fields[-1]
        homes[fields[0]] = fields[-2]

with open('/etc/shadow', 'r') as file_object:
    shadow_pass = {}
    for line in file_object:
        line = line.strip()
        if line == '':
            continue
        fields = line.split(":")
        shadow_pass[fields[0]] = fields[1]


print('')

print('')

for user in homes:
    try:
        with open(homes[user] + '/.ssh/authorized_keys') as authorized_keys:
            auth_keys[user] = []
            for line in authorized_keys:
                auth_keys[user].append(line[-48:].strip('\n'))

        if bool(auth_keys[user]):
            print('{}\n------'.format(user))
            print('Home:  {}'.format(homes[user]))
            print('Shell: {}'.format(shell[user]))
            print('Pass:  {}'.format(shadow_pass[user][:32]))
            print('Keys:  {}'.format(auth_keys[user]))
            print('\n')
    except:
        continue

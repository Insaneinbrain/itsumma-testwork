# Тестовое задание ITsumma.

### Создание ВМ в Selectel.
1. Создал ВМ в UCP Selectel, согласно указанным требованиям.
2. При создании ВМ указал публичный SSH-ключ.
### Настройка ВМ средствами Ansible.
1. На клиентской машине установил Ansible.
2. На клиентской машине загрузил через SFTP предоставленный архив common-master(1).tar.gz и распаковал его командой ```tar -xvzf common-master\(1\).tar.gz```
3. Рядом с директорией common-master создал 2 файла inventory.yml (файл инвентаризации) и common-master.yml (плейбук).
4. Запустил выполнение плейбука командой ```ansible-playbook common-master.yml -i inventory.yml```
5. При выполнении плейбука задача Creating sysctl.conf вызвала ошибку: ```cannot stat /proc/sys/vm/swapiness: No such file or directory```
6. В файле ./common-master/defaults/main.yml vm.swapiness меняем на vm.swappiness и его значение test меняем на 10.
7. Далее была ошибка ```failed: [robin] (item={'name': 'net.ipv4.netfilter.ip_conntrack_max', 'value': 1548576}) => {"ansible_loop_var": "item", "changed": false, "item": {"name": "net.ipv4.netfilter.ip_conntrack_max", "value": 1548576}```. Проверяем, что система включает поддержку nf_conntrack командой ```lsmod | grep nf_conntrack``` включаем модуль командой ```modprobe nf_conntrack``` и добавляем модуль в автозагрузку командой ```echo nf_conntrack >> /etc/modules-load.d/nf_conntrack.conf```.
8. Осталась ошибка ```failed: [robin] (item={'name': 'net.ipv4.netfilter.ip_conntrack_max', 'value': 1548576}) => {"ansible_loop_var": "item", "changed": false, "item": {"name": "net.ipv4.netfilter.ip_conntrack_max", "value": 1548576} Проверяем, поддерживает ли ядро параметр net.ipv4.netfilter.ip_conntrack_max``` с помощью команды ```sysctl -a | grep net.ipv4.netfilter.ip_conntrack_max``` из вывода видно, что параметр не поддерживается, поэтому комментируем изменения по данному модулю в файле ./common-master/defaults/main.yml
9. Далее возникает ошибка ```fatal: [robin]: FAILED! => {"changed": false, "msg": "Could not find the requested service firewalld: host"}``` возникает она по причине не установленного пакета firewalld можно установить вручную, можно дописать в задачу установку firewalld, можно отключить задачу остановки сервиса firewalld.
10. При очередном запуске появляется ошибка ```fatal: [robin]: FAILED! => {"changed": false, "msg": "AnsibleUndefinedVariable: 'ansible_ssh_port' is undefined"}``` Решается добавлением в файл inventory.yml параметра ```ansible_ssh_port=22``` у хоста.
11. После возникает ошибка ``` `fatal: [robin]: FAILED! => {"msg": "The conditional check 'project != ''' failed. The error was: error while evaluating conditional (project != ''): 'project' is undefined\n\nThe error appears to be in '/root/itsumma/common-master/tasks/helpers.yml': line 2, column 3, but may\nbe elsewhere in the file depending on the exact syntax problem.\n\nThe offending line appears to be:\n\n---\n- name: Updating hosts info...\n  ^ here\n"}``` она исправляется добавлением переменной project в файл inventory.yml.
12. При следующем запуске возникла ошибка ```failed: [robin] (item=environment.inc) => {"ansible_loop_var": "item", "changed": false, "item": "environment.inc", "msg": "AnsibleUndefinedVariable: 'itsumma_host' is undefined"}``` которую удалось убрать с помощью объявления переменной itsumma_host в файле inventory.yml.

### Установка на сервер bitrixenv, bitrix.

1. Обновляем все пакеты системы командой ```yum clean all && yum update```.
2. Загружаем скрипт установки и запускаем его командами ```wget https://repo.bitrix.info/yum/bitrix-env.sh && chmod +x bitrix-env.sh && ./bitrix-env.sh``` дожидаемся окончания установки.
3. Отключаемся от ssh сервера и заходим заново для того чтобы установить новый пароль для пользователя bitrix. Устанавливаем новый пароль.
4. Создаем пул через пункт 1. Create Management pool of server.
5. Открываем http://45.12.231.226/index.php и устанавливаем Bitrix.

### Вместо апача поставить php-fpm, настроить работу свежеподнятого сайта через связку nginx-fpm.

1. Устанавливаем php командой ```yum install -y php-cli php-fpm php-common php-curl php-gd php-imap php-intl php-mbstring php-xml php-zip php-bz2 php-bcmath php-json php-opcache php-devel php-mysqlnd``` .
2. Запускаем php-fpm командой ```systemctl enable --now php-fpm```.
3. Проверяем статус php-fpm командой ```systemctl status php-fpm```.
4. Проверим список запущенных пулов командой ```ps -ef |grep "[p]hp-fpm: pool"```.
5. Меняем пользователя от имени которой работает пул www командой ```sed -i "s/user = apache/user = bitrix/g" /etc/php-fpm.d/www.conf```.
6. Меняем группу от имени которой работает пул www командой ```sed -i "s/group = apache/group = bitrix/g" /etc/php-fpm.d/www.conf```.
7. Перезапускаем php-fpm командой ```systemctl restart php-fpm```.
8. В директории /etc/nginx/bx/site_avaliable создаем новый конфиг mysite.conf
9. Создадим симлинк на файл /etc/nginx/bx/site_avaliable/mysite.conf в директории /etc/nginx/bx/site_enabled командой ```ln -s /etc/nginx/bx/site_avaliable/mysite.conf /etc/nginx/bx/site_enabled/100-mysite.conf```.
Содержимое mysite.conf:
```
server {
    listen 81;
    server_name _;
    root        /home/bitrix/www;
.....
    location / {
        allow 127.0.0.1;
        deny all;
        index   index.html index.php;
        try_files $uri $uri/ /bitrix/urlrewrite.php$is_args$args;
    }
......
    location ~* \.(gif|jpg|png)$ {
        allow 127.0.0.1;
        deny all;
        expires 30d;
    }
...
    location ~ \.php$ {
        allow 127.0.0.1;
        deny all;
        fastcgi_pass  127.0.0.1:9000;
        fastcgi_index  index.php;
        fastcgi_param  SCRIPT_FILENAME $document_root$fastcgi_script_name;
        include fastcgi_params;
    }
}
```
10. В файле /etc/nginx/bx/sites_available/s1.conf и /etc/nginx/bx/site_avaliable/ssl.s1.conf в параметре set $proxyserver меняем адрес на http://127.0.0.1:81.
11. Проверяем конфигурацию nginx командой ```nginx -t``` и если нет ошибок, то перезагружаем nginx командой ```nginx -s reload```.

### Установить elk в докере, закрыть паролем, настроить сбор логов nginx, fpm в elk, дать ссылку на веб-интерфейс.

1. Устанавливаем Docker выполнив несколько команд:
```
yum install -y yum-utils
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
yum install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin -y
systemctl enable docker
systemctl start docker
```
2. Устанавливаем Docker-compose выполнив несколько команд:
```
curl -SL https://github.com/docker/compose/releases/download/v2.24.1/docker-compose-linux-x86_64 -o /usr/local/bin/docker-compose
ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
chmod +x /usr/local/bin/docker-compose
```
3. Устанавливаем дополнительные необходимые пакеты командой ```yum -y install curl git```.
4. Клонируем с Github репозиторий для работы стека ELK командой ```git clone https://github.com/deviantony/docker-elk.git```.
5. Настраиваем Elasticsearch редактируя файл конфигурации elasticsearch/config/elasticsearch.yml меняем в параметре xpack.license.self_generated.type значение на basic.
6. Меняем логины и пароли, редактируя файл .env и версию ELK на 8.7.1.
7. Из-за блокировки РФ IP меняем репозиторий docker.elastic.co на docker.io в файлах ./elasticsearch/Dockerfile ./logstash/Dockerfile и ./kibana/Dockerfile Точные адреса репозиториев можно получить выполнив команды ```docker pull elasticsearch:8.7.1``` ```docker pull logstash:8.7.1``` ```docker pull kibana:8.7.1```.
8. Запускаем ELK в контейнере выполнив команду ```docker-compose up -d```.
9. Заходим на страницу kibana http://45.12.231.226:5601 и логинимся с данными, которые указывали в .env.
10. На главной странице нажимаем Add integrations в поиске вводим nginx logs выбираем Nginx Logs выбираем вкладку Linux RPM и следуя инструкции устанавливаем Filebeat.
11. На главной странице нажимаем Add integrations в поиске вводим php выбираем PHP-FPM Metrics выбираем вкладку Linux RPM и следуя инструкции устанавливаем и настраиваем Metricbeat. Потребуется изменить конфиг nginx и php-fpm для вывода метрик php-fpm на странице http://127.0.0.1/status, а именно в /etc/nginx/bx/sites_available/s1.conf добавить: 
```
location ~ /status {
    allow 127.0.0.1;
    deny all;
    include fastcgi_params;
    fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
    fastcgi_pass 127.0.0.1:9000;
}
```
и перезапустить Nginx командой ```nginx -s reload```.

### Установить гитлаб, завести в нем пользователя itsumma и проект itsumma.

1. Устанавливаем Gitlab командами:
```
curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ce/script.rpm.sh | sudo bash
yum install gitlab-ce -y
```
2. В файле /etc/gitlab/gitlab.rb изменим в параметре external_url значение на 'http://45.12.231.226:85' и применим изменения командой ```gitlab-ctl reconfigure```.
3. Меняем пароль на учетную запись root в gitlab командой ```gitlab-rake "gitlab:password:reset[root]"```.
4. Авторизуемся под учетной запись root с новым паролем в gitlab http://45.12.231.226:85.
5. Создаем пользователя ITsumma и авторизуемся под ним.
6. Создаем репозиторий ITsumma и загружаем в него Ansible плейбук с ролью и текущее описание в файле README.md.

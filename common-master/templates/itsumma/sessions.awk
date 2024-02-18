/sess_/ {
    load_sessions[$9]++;
    if (load_sessions[$9]>max_sess_link_count){
        max_sess_link_count = load_sessions[$9];
        max_sess_link_name = $9;
    };

    if ($4 ~ /.*uW$/ ){ locked_id[$9]=$2 };
}

END {

    print max_sess_link_count, max_sess_link_name,locked_id[max_sess_link_name];

    if (locked_id[max_sess_link_name] && max_sess_link_count>3) {
        #    r=system("kill "locked_id[max_sess_link_name]);
        #    if (!r) print "Locking process "locked_id[max_sess_link_name]" killed"
        system("ls -al "max_sess_link_name);
    }

}
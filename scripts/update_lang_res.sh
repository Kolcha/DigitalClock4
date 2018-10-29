#!/bin/bash

proj_dirs=$(find . -name "*.pro" -exec dirname {} \;)

for d in $proj_dirs
do
    lang_dir="$d/lang"
    [[ -d $lang_dir ]] || continue

    lang_pri="$lang_dir/lang.pri"
    lang_qrc="$lang_dir/lang.qrc"

    echo "TRANSLATIONS += \\" > $lang_pri

    echo "<RCC>" > $lang_qrc
    echo "    <qresource prefix=\"/$(basename $d)/lang\">" >> $lang_qrc

    lang_dir_name=$(basename $lang_dir)
    for t in $(ls -1 $lang_dir/*.ts)
    do
        ts_file=$(basename $t)
        echo "    $lang_dir_name/$ts_file \\" >> $lang_pri
        qm_file=$(echo $ts_file | sed 's/\.ts/\.qm/g')
        echo "        <file>$qm_file</file>" >> $lang_qrc
    done

    echo "    </qresource>" >> $lang_qrc
    echo "</RCC>" >> $lang_qrc

    sed -i.bak '$ s/ \\$//' $lang_pri
    rm -f "$lang_pri.bak"

    echo "" >> $lang_pri
    echo "RESOURCES += $lang_dir_name/$(basename $lang_qrc)" >> $lang_pri
done

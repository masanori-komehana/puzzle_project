import sqlite3
import csv
import os

#######↓ここのパラメータを変える↓#######
dbname = ''
target_table_name = ''
import_table_name = ''
is_create_table = 
is_header_skip = 
#####################################


#######↓インポート先のテーブルDDL↓#######
sql_script = """

"""
#######################################

class ImportSQLite():
    def __init__(self, dbname, target_table_name, import_data_name, is_create_table, is_header_skip=False, sql_create_table=None):
        """
        csvまたはtsvファイルをSQLiteへインポートする
        :param dbname: text 接続先DB名
        :param target_table_name: text インポート先となるDB上のテーブル名
        :param import_data_name: text インポートしたいデータ名
        :param is_create_table: boolean インポート先となるテーブルを作成するか否か
        :param is_header_skip: boolean インポートするデータのヘッダーを読み飛ばすか否か
        :param sql_create_table: text インポート先となるテーブルのDDL
        """
        self.dbname = dbname
        self.target_table_name = target_table_name
        self.import_data_name = import_data_name
        self.is_create_table = is_create_table
        self.is_header_skip = is_header_skip
        _, raw_delimiter = os.path.splitext(import_data_name)
        if raw_delimiter == '.csv':
            self.delimiter = ','
        elif raw_delimiter == '.tsv':
            self.delimiter = '\t'
        else:
            raise ValueError('Import file should be csv or tsv.')

        if is_create_table:
            if not sql_create_table:
                raise ValueError('It\'s necessary of sql to create table')
            else:
                self.sql_create_table = sql_create_table


    def read_import_file(self):
        with open(self.import_data_name, 'r', encoding='utf-8') as f:
            reader = csv.reader(f, delimiter=self.delimiter)
            if self.is_header_skip:
                header = next(reader)

            return [i for i in reader]


    def pick_column_num(self, import_data):
        """
        インポートファイルの列数を算出する
        :param import_data: array(two-dimensional)
        :return: int
        """
        columns = []
        for raw in import_data:
            columns.append(len(raw))
        if len(set(columns)) == 1:
            return columns[0]
        else:
            raise ValueError('this import files has diffrenect column numbers.')


    def insert_csv_file(self):
        input_file = self.read_import_file()
        column = self.pick_column_num(input_file)
        val_questions = ['?' for i in range(column)]
        cur.executemany("insert into {0} values ({1})".format(self.target_table_name, ','.join(val_questions)), input_file)


if __name__ == '__main__':

    sql = ImportSQLite(
        dbname=dbname,
        target_table_name=target_table_name,
        import_data_name=import_table_name,
        is_create_table=is_create_table,
        is_header_skip= is_header_skip,
        sql_create_table=sql_script
    )

    conn = sqlite3.connect(sql.dbname)
    cur = conn.cursor()

    if sql.is_create_table:
        cur.execute('drop table if exists {};'.format(target_table_name))
        cur.execute(sql.sql_create_table)

    sql.insert_csv_file()

    conn.commit()
    conn.close()



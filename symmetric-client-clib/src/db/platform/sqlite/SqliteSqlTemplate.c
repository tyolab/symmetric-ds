/**
 * Licensed to JumpMind Inc under one or more contributor
 * license agreements.  See the NOTICE file distributed
 * with this work for additional information regarding
 * copyright ownership.  JumpMind Inc licenses this file
 * to you under the GNU General Public License, version 3.0 (GPLv3)
 * (the "License"); you may not use this file except in compliance
 * with the License.
 *
 * You should have received a copy of the GNU General Public License,
 * version 3.0 (GPLv3) along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include <db/sqlite/SqliteSqlTemplate.h>

SymList * SymSqliteSqlTemplate_query(SymSqliteSqlTemplate *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error, void *map_row(SymRow *row)) {
    sqlite3_stmt *stmt;
    printf("Preparing %s\n", sql);
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }

    // TODO: do we need to convert to sqlType and bind correctly?
    printf("Binding [");
    int i;
    for (i = 0; args != NULL && i < args->size; i++) {
        //if (argLengths != NULL) {
        //    sqlite3_bind_text(stmt, i + 1, args[i], argLengths[i], SQLITE_STATIC);
        //} else {
        sqlite3_bind_text(stmt, i + 1, args->get(args, i), -1, SQLITE_STATIC);

        if (i > 0) {
            printf(",");
        }
        printf("%s", args->get(args, i));
    }
    printf("]\n");

    SymList *list = SymList_new(NULL);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int i, type, size, columnCount = sqlite3_column_count(stmt);
        SymRow *row = SymRow_new(NULL, columnCount);
        char *name, *value;
        for (i = 0; i < columnCount; i++) {
            name = (char *) sqlite3_column_name(stmt, i);
            type = sqlite3_column_type(stmt, i);
            size = sqlite3_column_bytes(stmt, i) + 1;
            value = (char *) sqlite3_column_text(stmt, i);
            row->put(row, name, value, type, size);
        }
        void *object = map_row(row);
        list->add(list, object);
        // TODO: need to destroy each row, and the row mapper needs to make a copy
        //row->destroy(row);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
    sqlite3_finalize(stmt);
    *error = 0;
    return list;
}

static SymRow * SymSqliteSqlTemplate_row_mapper(SymRow *row) {
    return row;
}

SymRow * SymSqliteSqlTemplate_query_for_list(SymSqliteSqlTemplate *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    return (SymRow *) SymSqliteSqlTemplate_query(this, sql, args, sqlTypes, error, (void *) SymSqliteSqlTemplate_row_mapper);
}

int SymSqliteSqlTemplate_query_for_int(SymSqliteSqlTemplate *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }

    int i;
    for (i = 0; args != NULL && i < args->size; i++) {
        //if (argLengths != NULL) {
        //    sqlite3_bind_text(stmt, i + 1, args[i], argLengths[i], SQLITE_STATIC);
        //} else {
        sqlite3_bind_text(stmt, i + 1, args->get(args, i), -1, SQLITE_STATIC);
    }

    int value = 0;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        value = sqlite3_column_int(stmt, 0);
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
    sqlite3_finalize(stmt);
    *error = 0;
    return value;
}

char * SymSqliteSqlTemplate_query_for_string(SymSqliteSqlTemplate *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }

    int i;
    for (i = 0; args != NULL && i < args->size; i++) {
        //if (argLengths != NULL) {
        //    sqlite3_bind_text(stmt, i + 1, args[i], argLengths[i], SQLITE_STATIC);
        //} else {
        sqlite3_bind_text(stmt, i + 1, args->get(args, i), -1, SQLITE_STATIC);
    }

    char *value = NULL;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        value = (char *) sqlite3_column_text(stmt, 0);
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
    sqlite3_finalize(stmt);
    *error = 0;
    return value;
}

int SymSqliteSqlTemplate_update(SymSqliteSqlTemplate *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    sqlite3_stmt *stmt;
    printf("Executing: %s\n", sql);
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }

    int i;
    for (i = 0; args != NULL && i < args->size; i++) {
        //if (argLengths != NULL) {
        //    sqlite3_bind_text(stmt, i + 1, args[i], argLengths[i], SQLITE_STATIC);
        //} else {
        sqlite3_bind_text(stmt, i + 1, args->get(args, i), -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
    sqlite3_finalize(stmt);
    *error = 0;
    return sqlite3_changes(this->db);
}

SymSqlTransaction * SymSqliteSqlTemplate_start_sql_transaction(SymSqliteSqlTemplate *this) {
    return (SymSqlTransaction *) SymSqliteSqlTransaction_new(NULL, this);
}

void SymSqliteSqlTemplate_destroy(SymSqliteSqlTemplate *this) {
    free(this);
}

SymSqliteSqlTemplate * SymSqliteSqlTemplate_new(SymSqliteSqlTemplate *this, sqlite3 *db) {
    if (this == NULL) {
        this = (SymSqliteSqlTemplate *) calloc(1, sizeof(SymSqliteSqlTemplate));
    }
    this->db = db;
    SymSqlTemplate *super = (SymSqlTemplate *) this;
    super->query_for_int = (void *) &SymSqliteSqlTemplate_query_for_int;
    super->query_for_string = (void *) &SymSqliteSqlTemplate_query_for_string;
    super->query_for_list = (void *) &SymSqliteSqlTemplate_query_for_list;
    super->query = (void *) &SymSqliteSqlTemplate_query;
    super->update = (void *) &SymSqliteSqlTemplate_update;
    super->start_sql_transaction = (void *) &SymSqliteSqlTemplate_start_sql_transaction;
    this->destroy = (void *) &SymSqliteSqlTemplate_destroy;
    return this;
}
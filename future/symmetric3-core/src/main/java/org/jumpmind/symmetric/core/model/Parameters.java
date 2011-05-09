package org.jumpmind.symmetric.core.model;

import java.util.HashMap;
import java.util.Properties;

import org.jumpmind.symmetric.core.sql.SqlConstants;

public class Parameters extends HashMap<String, String> {

    private static final long serialVersionUID = 1L;

    public final static String DB_METADATA_IGNORE_CASE = "db.metadata.ignore.case";

    public final static String DB_USE_ALL_COLUMNS_AS_PK_IF_NONE_FOUND = "db.pk.use.all.if.none";

    public final static String DB_USE_PKS_FROM_SOURCE = "db.pk.use.from.source";

    public final static String DB_SUPPORT_BIG_LOBS = "db.support.big.lobs";

    public final static String DB_QUERY_TIMEOUT = "db.sql.query.timeout.seconds";

    public final static String DB_STREAMING_FETCH_SIZE = "db.jdbc.streaming.results.fetch.size";

    public Parameters() {
    }

    public Parameters(Properties properties) {
        for (Object key : properties.keySet()) {
            put((String) key, properties.getProperty((String) key));
        }
    }

    public long getLong(String key, long defaultValue) {
        long returnValue = defaultValue;
        String value = get(key);
        if (value != null) {
            try {
                returnValue = Long.parseLong(value);
            } catch (NumberFormatException ex) {
                // TODO log error
            }
        }
        return returnValue;
    }

    public int getInt(String key, int defaultValue) {
        int returnValue = defaultValue;
        String value = get(key);
        if (value != null) {
            try {
                returnValue = Integer.parseInt(value);
            } catch (NumberFormatException ex) {
                // TODO log error
            }
        }
        return returnValue;
    }

    public boolean is(String key, boolean defaultValue) {
        boolean returnValue = defaultValue;
        String value = get(key);
        if (value != null) {
            returnValue = Boolean.parseBoolean(value);
        }
        return returnValue;
    }

    public int getQueryTimeout() {
        return getInt(DB_QUERY_TIMEOUT, SqlConstants.DEFAULT_QUERY_TIMEOUT_SECONDS);
    }

    public int getStreamingFetchSize() {
        return getInt(DB_STREAMING_FETCH_SIZE, SqlConstants.DEFAULT_STREAMING_FETCH_SIZE);
    }

}

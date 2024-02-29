/* Created on Jan 21, 2005 */
package edu.vanderbilt.isis.util;

import java.util.logging.Formatter;
import java.util.logging.LogRecord;

/**
 * @author David Hanak
 */
public class ModestFormatter extends Formatter {
    /* (non-Javadoc)
     * @see java.util.logging.Formatter#format(java.util.logging.LogRecord)
     */
    public String format(LogRecord record) {
        return record.getMessage() + "\n";
    }
}

/**
 * Created on Jan 6, 2005
 */
package edu.vanderbilt.isis.udm;

/**
 * A user friendly class for exceptions thrown in UDM
 * 
 * @author Himanshu Neema
 */
public class UdmException extends Exception {
    String lastError;
    String message;
    
    /**
     * Creates a new exception based on a C++ error message.
     */
    public UdmException(String message) {
        this(message, false);
    }

    /**
     * 
     * @param fromJava If true, the error string will not be fetched from C++.
     */
    public UdmException(String message, boolean fromJava) {
        if (!fromJava) {
            // The root of the error is in the C++ UDM library, fetch the error code.
            try {
                lastError = UdmHelper.GetLastError();
            } catch (Throwable e) {
                throw new RuntimeException("Error while fetching last error from UDM", e);
            }
            this.message = message + ", UDMERROR: " + lastError;
        } else {
            // This error originated from the Java side
            this.message = message;
        }                
    }
    
    public UdmException(String message, Throwable cause) {
        super(message, cause);
        this.message = message;
    }
    
    /**
     * @see java.lang.Throwable#getMessage()
     */
    public String getMessage() {
        return message;
    }
}

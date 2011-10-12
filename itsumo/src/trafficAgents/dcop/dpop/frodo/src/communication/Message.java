package frodo.communication;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */
public class Message {
	private String payload;//payload

	private int type;

	private int senderId;

	private int destinationId;

	private long timestamp;//used for time monitoring

	private long messageId;//the ID of this message; currently unused, maybe later will implement some message tracking

	/**
	 * Constructor for Message.
	 */
	public Message(String s, int type, int snd, int dest) {
		this(s, 0, type, snd, dest, 0);
	}

	public Message(String s, long id, int type, int snd, int dest, long time) {
		this.payload = s;
		this.messageId = id;
		this.type = type;
		this.senderId = snd;
		this.destinationId = dest;
		this.timestamp = time;
	}

	/*	
	 public Message(String s, int type) {
	 this(s, type, -1, -1);
	 }
	 */
	/**
	 * Returns the payload.
	 * 
	 * @return String
	 * 
	 * @uml.property name="payload"
	 */
	public String getPayload() {
		return payload;
	}

	/**
	 * Sets the payload.
	 * 
	 * @param payload
	 *            The payload to set
	 * 
	 * @uml.property name="payload"
	 */
	public void setPayload(String message) {
		this.payload = message;
	}

	public String toString() {
		return payload;
	}

	public String toStringComplete() {
		return messageId + "\t" + timestamp + "\t" + type + "\t[" + senderId + "->" + destinationId + "]\t" + payload.toString();
		//		return "<" + type + ">\t" + "[" + senderId + "->" + destinationId + "]\t" + payload;
	}

	/**
	 * Returns the type.
	 * 
	 * @return int
	 * 
	 * @uml.property name="type"
	 */
	public int getType() {
		return type;
	}

	/**
	 * Sets the type.
	 * 
	 * @param type
	 *            The type to set
	 * 
	 * @uml.property name="type"
	 */
	public void setType(int type) {
		this.type = type;
	}

	/**
	 * @return Returns the destinationId.
	 * 
	 * @uml.property name="destinationId"
	 */
	public int getDestinationId() {
		return destinationId;
	}

	/**
	 * @param destinationId
	 *            The destinationId to set.
	 * 
	 * @uml.property name="destinationId"
	 */
	public void setDestinationId(int destinationId) {
		this.destinationId = destinationId;
	}

	/**
	 * @return Returns the senderId.
	 * 
	 * @uml.property name="senderId"
	 */
	public int getSenderId() {
		return senderId;
	}

	/**
	 * @param senderId
	 *            The senderId to set.
	 * 
	 * @uml.property name="senderId"
	 */
	public void setSenderId(int senderId) {
		this.senderId = senderId;
	}

	public long getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	public long getMessageId() {
		return messageId;
	}

	public void setMessageId(long messageId) {
		this.messageId = messageId;
	}

	public Object clone() {
		Message myClone = new Message(new String(payload), messageId, type, senderId, destinationId, timestamp);
		return myClone;
	}
}
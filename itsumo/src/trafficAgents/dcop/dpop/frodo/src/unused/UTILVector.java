/*
 * Created on Jun 6, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.unused;

import frodo.OCOP.MessageContext;
import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 * 
 * TODO To change the template for this generated type comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class UTILVector {

	int[] utils;

	int senderId;

	int receiverId;

	MessageContext context;

	public UTILVector(int[] ut, int snd, int rec, MessageContext mc) {
		this.utils = ut;
		this.senderId = snd;
		this.receiverId = rec;
		this.context = mc;
	}

	//-----------------------------------------------------------------------------------------------------------------//
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

	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the utils.
	 * 
	 */
	public int[] getUtils() {
		return utils;
	}

	/**
	 * @param utils
	 *            The utils to set.
	 * 
	 */
	public void setUtils(int[] utils) {
		this.utils = utils;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	/**
	 * @return Returns the context.
	 * 
	 */
	public MessageContext getContext() {
		return context;
	}

	/**
	 * @param context The context to set.
	 * 
	 * @uml.property name="context"
	 */
	public void setContext(MessageContext context) {
		this.context = context;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public String toString() {
		String str = new String("[" + senderId + "->" + receiverId + "] " + context.toString() + " " + HelperUtilities.arrayToString(utils));
		return str;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static UTILVector fromString(String content, int senderId, int receiverId) {
		UTILVector uv = null;
		String str = content.trim();
		String[] parts = str.split(":");
		if (parts.length == 2) {
			MessageContext mc = MessageContext.fromString(parts[0]);
			int[] utilsArray = HelperUtilities.IntegerArrayFromString(parts[1]);
			if (utilsArray.length != 0) {
				uv = new UTILVector(utilsArray, senderId, receiverId, mc);
			}
		}
		return uv;
	}

	/**
	 * @return Returns the receiverId.
	 * 
	 * @uml.property name="receiverId"
	 */
	public int getReceiverId() {
		return receiverId;
	}

	/**
	 * @param receiverId The receiverId to set.
	 * 
	 * @uml.property name="receiverId"
	 */
	public void setReceiverId(int receiverId) {
		this.receiverId = receiverId;
	}

}
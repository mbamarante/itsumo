/*
 * Created on Apr 27, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP.communication;

import java.util.HashMap;


/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Dispatcher {
	HashMap routers;

	/**
	 * @param routers
	 */
	public Dispatcher(HashMap routers) {
		super();
		this.routers = routers;
	}

	public Dispatcher() {
		super();
		this.routers = new HashMap();
	}

	public void addRouter(Router router) {
		routers.put(router.getIdResponsible(), router);
	}

	public Router getRouterByID(Integer rid) {
		return (Router) routers.get(rid);
	}

	public Router getRouterForTarget(Integer rid) {
		Object[] vals = routers.values().toArray();
		for (int i = 0; i < vals.length; i++)
			if (((Router) vals[i]).containsTarget(rid.intValue()))
				return (Router) vals[i];
		return null;
	}
}

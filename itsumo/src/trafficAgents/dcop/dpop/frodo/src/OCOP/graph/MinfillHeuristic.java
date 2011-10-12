/*
 * Created on Mar 13, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.graph
 */
package frodo.OCOP.graph;

import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.CSP.BinaryConstraint;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPAbstractProblem;

public class MinfillHeuristic {

	public static Vector bestOrdering(OCOPAbstractProblem problem) {
		OCOPAbstractProblem clona = (OCOPAbstractProblem) problem.clone();
		Vector ordering = new Vector();
		for (int i = 0; i < clona.getNumberOfNodes(); i++) {
			OCOPAbstractNode selected = getNext(clona, ordering);
			ordering.add(selected);
			for (int j = 0; j < selected.getNumberOfNeighbours() - 1; j++) {
				for (int k = j + 1; k < selected.getNumberOfNeighbours(); k++) {
					OCOPAbstractNode srcngh = selected.getNeighborByIndex(j);
					OCOPAbstractNode targngh = selected.getNeighborByIndex(k);
					if (srcngh.getNeighborById(targngh.getId()) == null) {
						BinaryConstraint bc = new ValuedConstraint(srcngh.getVariable(), targngh.getVariable());
						clona.addConstraint(bc);
						srcngh.addConstraint(bc);
						srcngh.addNeighbour(targngh);
						bc = new ValuedConstraint(targngh.getVariable(), srcngh.getVariable());
						targngh.addConstraint(bc);
						targngh.addNeighbour(srcngh);
					}
				}
			}
		}
		System.out.println(ordering);
		Vector rez = new Vector();
		for (int i = 0; i < ordering.size(); i++) {
			rez.add(problem.getNodeById(((OCOPAbstractNode) ordering.elementAt(i)).getId()));
		}
		return rez;
	}

	private static OCOPAbstractNode getNext(OCOPAbstractProblem problem, Vector selected) {
		OCOPAbstractNode nextBest = null;
		int bestScore = Integer.MAX_VALUE;
		for (int i = 0; i < problem.getNumberOfNodes(); i++) {
			OCOPAbstractNode curn = problem.getNodeByIndex(i);
			if (!selected.contains(curn)) {
				int score = getFillScore(problem, curn, selected);
				if (bestScore > score) {
					nextBest = curn;
					bestScore = score;
				}
			}
		}
		return nextBest;
	}

	private static int getFillScore(OCOPAbstractProblem problem, OCOPAbstractNode node, Vector visited) {
		int score = 0;
		for (int i = 0; i < node.getNumberOfNeighbours(); i++) {
			OCOPAbstractNode neigh = node.getNeighborByIndex(i);
			if (!visited.contains(neigh))
				score++;
		}
		return score;
	}
}

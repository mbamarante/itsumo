package frodo.OCOP;

import frodo.COP.OCOPProblemFactory;
import frodo.COP.Solution;
import frodo.CSP.Assignment;

public class TestSolution {
	public TestSolution() {
	}

	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: TestSolution solution_file_1 solution_file_");
			System.exit(0);
		}
		String problem_file = args[0];
		String solution_file1 = args[1];
		String solution_file2 = args[2];

		//build an OCOPProblem object from the input text file
		OCOPAbstractNode dummyNode = new OCOPBasicNode(null, 0, null);
		OCOPBasicProblem myProblem = (OCOPBasicProblem) OCOPProblemFactory.loadFromTextFile(problem_file, dummyNode);

		Solution solution1 = Solution.loadFromFile(solution_file1);
		Solution solution2 = Solution.loadFromFile(solution_file2);

		System.out.println("Solution 1:" + solution1.toString());
		System.out.println("Solution 2:" + solution2.toString());
		Assignment[] differences = solution1.getDifferences(solution2);
		System.out.println(differences);
		myProblem.setCurrentAssignments(solution1.getAssignment());
		double valuation1 = myProblem.getTotalEvaluation();
		myProblem.setCurrentAssignments(solution2.getAssignment());
		double valuation2 = myProblem.getTotalEvaluation();
		if (valuation1 != valuation2)
			System.out.println("Different valuations: Sol1=" + valuation1 + "\tSol2=" + valuation2);
		else {
			if (differences.length > 0)
				System.out.println("Even if the solutions are different, they yield the same utility=" + valuation1);
			else
				System.out.println("Identical solutions.");
		}
	}
}
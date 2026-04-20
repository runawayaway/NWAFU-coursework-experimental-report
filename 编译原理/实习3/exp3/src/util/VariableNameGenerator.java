package util;

/**
 * 变量名产生器
 * 
 */
public class VariableNameGenerator {
	// 序号
	public static int sequenceId = 0;
	private static final String VAR_PREFIX = "T";

	public static String genVariableName() {
		++sequenceId;
		return VAR_PREFIX + sequenceId;
	}

	public void clear() {
		// TODO Auto-generated method stub
		sequenceId = 0;
	}
}

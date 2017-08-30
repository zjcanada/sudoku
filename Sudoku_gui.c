/* This is a GUI Sudoku.
 *
 * Install Eclipse for java development
 * Install WindowBuilder for Eclipse
 * New WindowBuilder project
 *
 * After key in initial digits, click "Fix" to fix them.
 * Then click "Calc" to get the solution.
 */

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.*;

public class Sudoku_gui {

	protected Shell shell;
	private Text[][] texts = new Text[9][9];
	//private Text text;
	private int[][] matrix = new int[9][9];
/*			new int[][]	{
			{ 7, 0, 6, 2, 0, 5, 0, 0, 0 },
			{ 9, 0, 2, 0, 0, 7, 0, 0, 5 },
			{ 0, 0, 0, 0, 0, 3, 0, 0, 0 },
			{ 0, 0, 1, 0, 0, 0, 9, 0, 0 },
			{ 0, 6, 8, 0, 4, 0, 2, 3, 0 },
			{ 0, 0, 3, 0, 0, 0, 7, 0, 0 },
			{ 0, 0, 0, 6, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 3, 0, 0, 4, 0, 6 },
			{ 0, 0, 0, 9, 0, 8, 3, 0, 7 }
	};*/
	private boolean[][] fixed = new boolean[9][9];
	
	int row_adding(int n) { return n / 3; }

	int col_adding(int n) { return n % 3; }

	boolean collision_square(int i, int j)
	{
	    // (i,j) can only be:
	    // (0,0), (0,3), (0,6)
	    // (3,0), (3,3), (3,6)
	    // (6,0), (6,3), (6,6)

	    int row_adding_k, col_adding_k;
	    int row_adding_m, col_adding_m;
	    int k, m;

	    for (k=0; k<8; ++k) {
	        row_adding_k = row_adding(k);
	        col_adding_k = col_adding(k);
	        for (m=k+1; m<9; ++m) {
	            row_adding_m = row_adding(m);
	            col_adding_m = col_adding(m);
	            if (matrix[i+row_adding_k][j+col_adding_k] == matrix[i+row_adding_m][j+col_adding_m]
	                && matrix[i+row_adding_k][j+col_adding_k] != 0)
	                return true;
	        }
	    }

	    return false;
	}

	boolean collision(int i, int j)
	{
	    int k, m;
	    int start_row, start_col;

	    // check the row i
	    for (k=0; k<8; ++k)
	        for (m=k+1; m<9; ++m)
	            if (matrix[i][k] == matrix[i][m] && matrix[i][k] != 0)
	                return true; // collision
	    // check the column j
	    for (k=0; k<8; ++k)
	        for (m=k+1; m<9; ++m)
	            if (matrix[k][j] == matrix[m][j] && matrix[k][j] != 0)
	                return true; // collision

	    // check the square
	    start_row = ((i) % 3)*3;
	    start_col = ((j) % 3)*3;
	    return collision_square(start_row, start_col);
	}

	void print_matrix()
	{
	    int i, j;

	    for (i=0; i<9; ++i) {
	        for (j=0; j<9; ++j) {
	        	if (! fixed[i][j])
	        		texts[i][j].setText(new Integer(matrix[i][j]).toString());
	        }
	    }
	}

	int test(int i, int j)
	{
	    int k;
	    int ret = 0;

	    if (i > 8) {
	        // solved
	        print_matrix();
	        //System.out.println("Solved!");
	        MessageBox messageBox = new MessageBox(shell, SWT.ICON_INFORMATION);
	        messageBox.setText("Info");
	        messageBox.setMessage("Found solution!");
	        messageBox.open();
	        return -1;
	    }

	    //System.out.println( "" + i + "," + j + ":");

	    if (!fixed[i][j]) {
	        for (k=0; k<9; ++k) {
	            matrix[i][j] = k + 1; /* try value 1, 2, ... 9 */
	            if (! collision(i, j)) {
	            	//System.out.println("trying: " + matrix[i][j]);
	                if (j < 8)
	                    ret = test(i, j+1);
	                else
	                    ret = test(i+1, 0);
	            }
	            if (ret < 0)
	            	return ret;
	        }
	        /* all collided, restore it to 0 */
	        matrix[i][j] = 0;
	    }
	    else {
	        if (j < 8)
	            ret = test(i, j+1);
	        else
	        	ret = test(i+1, 0);
	    }

	    if (i == 0 && j == 0 && ret == 0) {
	        MessageBox messageBox = new MessageBox(shell, SWT.ICON_WARNING);
	        messageBox.setText("Warn");
	        messageBox.setMessage("Not found solution!");
	        messageBox.open();
	    }
	    return ret;
	    //System.out.println("exiting... " + i + "," + j);
	}

	/**
	 * Launch the application.
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			Sudoku_gui window = new Sudoku_gui();
			window.open();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Open the window.
	 */
	public void open() {
		Display display = Display.getDefault();
		createContents();
		shell.open();
		shell.layout();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
	}

	/**
	 * Create contents of the window.
	 */
	protected void createContents() {
		shell = new Shell();
		shell.setSize(557, 566);
		shell.setText("Sudoku Application");
		
		//text = new Text(shell, SWT.BORDER);
		//text.addModifyListener(new ModifyListener() {
		//	public void modifyText(ModifyEvent arg0) {
		//	}
		//});
		//text.setBounds(20, 454, 76, 21);
		
		Button btnFix = new Button(shell, SWT.NONE);
		btnFix.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				int i, j;
				Button b = (Button)e.getSource();
				FontData fontData = b.getFont().getFontData()[0];
				Font font = new Font(Display.getDefault(), new FontData(fontData.getName(), fontData.getHeight(), SWT.BOLD));
				Color color = new Color(b.getBackground().getDevice(), 255, 255, 127);
				for (i=0; i<9; ++i) {
					for (j=0; j<9; ++j) {
						//System.out.print(matrix[i][j]);
						//System.out.print(' ');
						if (matrix[i][j] != 0)
						{
							fixed[i][j] = true;
							texts[i][j].setEditable(false);
							texts[i][j].setFont(font);
							texts[i][j].setBackground(color);
						}
					}
					//System.out.println();
				}
				b.setEnabled(false);
			}
		});
		btnFix.setBounds(114, 454, 75, 25);
		btnFix.setText("Fix");
		
		Button btnCalc = new Button(shell, SWT.NONE);
		btnCalc.setBounds(211, 454, 75, 25);
		btnCalc.setText("Calc");
		btnCalc.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				test(0, 0);
			}
		});
		
		abstract class ModifyListener1 implements ModifyListener {
			int row, col;
			ModifyListener1(int i, int j) { row=i; col=j; } 
		}
		
		for (int i=0; i<9; ++i) {
			for (int j=0; j<9; ++j){
				//matrix[i][j] = 0;
				int start_x = 30 + j*30 + (j/3) *10;
				int start_y = 30 + i*30 + (i/3) *10;
				texts[i][j] = new Text(shell, SWT.BORDER);
				texts[i][j].setTextLimit(1);
				if (matrix[i][j] != 0)
					texts[i][j].setText(new Integer(matrix[i][j]).toString());
				texts[i][j].setBounds(start_x, start_y, 25, 25);
				texts[i][j].addModifyListener(new ModifyListener1(i ,j) {
					public void modifyText(ModifyEvent arg0) {
						Text t = (Text)arg0.getSource();
						String s = t.getText();
						if (!s.isEmpty()) {
							char c = s.charAt(0);
							if (c < '0' || c > '9') {
								MessageBox messageBox = new MessageBox(shell, SWT.ICON_WARNING);
						        messageBox.setText("Warning");
						        messageBox.setMessage("Enter 1 to 9 only");
						        messageBox.open();
						        t.selectAll();
						        t.cut();
						        return;
							}
						}
				        if (t.getText().isEmpty())
							matrix[row][col] = 0;
						else
							matrix[row][col] = Integer.parseInt(t.getText());
					}
				});
			} // j
		} // i
	} // createContents
}


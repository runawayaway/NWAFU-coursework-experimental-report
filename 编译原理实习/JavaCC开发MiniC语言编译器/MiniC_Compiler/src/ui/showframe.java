package ui;


import java.awt.Color;
import java.awt.FileDialog;
import java.awt.Font;
import java.awt.Frame;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

import parser.*;

import javax.swing.JButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JLabel;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.awt.event.ActionEvent;

public class showframe extends JFrame {

	private JPanel contentPane;
	MyNewGrammar parser=null;
	FileInputStream fileinputStream =null;
	/**
	 * Create the frame.
	 * @throws FileNotFoundException 
	 */
	public showframe() throws FileNotFoundException {
		setTitle("编译器");
		setForeground(Color.RED);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(600, 100, 1200, 1200);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		
		JButton btnNewButton = new JButton("选择文件");
		btnNewButton.setBounds(20, 200, 100, 30);
		
		JButton btnNewButton_1 = new JButton("进行处理");
		btnNewButton_1.setBounds(20, 300, 100, 30);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(130, 50, 500, 1000);
		
		JScrollPane scrollPane_1 = new JScrollPane();
		scrollPane_1.setBounds(650, 50, 500, 1000);
		
		JLabel lblNewLabel = new JLabel("输入内容");
		lblNewLabel.setBounds(350, 20, 90, 20);
		
		JLabel lblNewLabel_1 = new JLabel("输出内容");
		lblNewLabel_1.setBounds(870, 20, 90, 20);
		
		contentPane.setLayout(null);
		contentPane.add(lblNewLabel);
		contentPane.add(btnNewButton);
		contentPane.add(lblNewLabel_1);
		contentPane.add(btnNewButton_1);
		contentPane.add(scrollPane);
		
		
		JTextArea text= new JTextArea();
		scrollPane.setViewportView(text);
		text.setFont(new Font("宋体",0,20));
		contentPane.add(scrollPane_1);
		
		JTextArea text1= new JTextArea();
		scrollPane_1.setViewportView(text1);
		text1.setFont(new Font("宋体",0,20));
		
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				text.setText("");
				Frame frame =new Frame("选择文件");
				FileDialog fopen =new FileDialog(frame,"选择文件",FileDialog.LOAD);
				fopen.setVisible(true);
				String filepath = fopen.getDirectory() + fopen.getFile();
				try 
				{
					InputStream in = new FileInputStream(filepath);
					InputStreamReader inr = new InputStreamReader(in, "UTF-8");
					BufferedReader br = new BufferedReader(inr);
					String str = null;
					str = br.readLine();
					while(str != null)
					{
						text.append(str + "\n");
						str = br.readLine();
					}
					br.close();
					inr.close();
					in.close();
				} catch (UnsupportedEncodingException | FileNotFoundException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
				} catch (IOException e1) {
						// TODO 自动生成的 catch 块
						e1.printStackTrace();
				}	
			}
		});
		
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String str = text.getText();
				File f = new File("Input.txt"); 
		        try
		        {
		            text1.setText("");
		            if(!f.exists())
		            {
		            	f.createNewFile();
		            }
					FileWriter fout = new FileWriter(f);	
					fout.write(str);
					fout.flush();
					fout.close();
					MyNewGrammar.dealData("Input.txt");
					if(MyNewGrammar.errorList.isEmpty() == false)
					{
						text1.append("出现报错提示：" + "\n");
						Iterator<String> itr =  MyNewGrammar.errorList.iterator();
						while (itr.hasNext()) 
						{
							String tmp = (String) itr.next();
							text1.append(tmp.toString()+"\n");
						}	
					}
					else
					{
						InputStream in = new FileInputStream("Output.txt");
						InputStreamReader inr = new InputStreamReader(in, "UTF-8");
						BufferedReader br = new BufferedReader(inr);
						str = br.readLine();
						while(str != null)
						{
							text1.append(str + "\n");
							str = br.readLine();
						}
						br.close();
						inr.close();
						in.close();
					}
			}catch (IOException e1) {
				// TODO 自动生成的 catch 块
				text1.setText(e1.getMessage());
			}
		}
		});
	}
}

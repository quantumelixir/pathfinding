import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.io.FileReader;
import java.lang.NumberFormatException;

public class EnlargeMap
{
	public String targetFile;
	public int factor;

	public EnlargeMap()
	{
		targetFile = null;
		factor = 3;
	}

	private void printHelp()
	{
		System.err.println("Usage: EnlargeMap -map <filename> -f[actor] <int>");
		System.err.println("Where:");
		System.err.println("<filename> is the name of a HOG-style map file");
		System.err.println("<int> is an enlargement factor by which to increase the "+
				"map size. Default = 3.");
	}

	public static void main(String[] args)
	{
		EnlargeMap enlarger = new EnlargeMap();
		enlarger.readArgs(args);
		System.out.println("map: "+enlarger.targetFile+" factor: "+enlarger.factor);
		try
		{
			enlarger.enlarge();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	private void enlarge() throws FileNotFoundException, IOException
	{
		File originalMap = new File(targetFile);
		String fname = originalMap.getParentFile().getAbsolutePath()+"/e"+factor+"x"+factor+"_"+originalMap.getName();
		System.out.println("proposed name: "+originalMap.getParentFile().getAbsolutePath()+"/"+fname);
		BufferedReader reader = new BufferedReader(
				new FileReader(new File(targetFile)));
		PrintWriter writer = new PrintWriter(
				new FileOutputStream(fname, false), true);

		String inLine = reader.readLine();
		while(inLine != null)
		{
			if(inLine.startsWith("map"))
			{
				writer.println("map");
				inLine = reader.readLine();
				continue;
			}
			if(inLine.startsWith("type"))
			{
				String[] tokens = inLine.split(" ");
				if(tokens[1].startsWith("octile"))
					writer.println("type octile");
				else
				{
					System.err.print("Error. Non-octile map type: "+tokens[1]);
					System.exit(22);
				}
				inLine = reader.readLine();
				continue;
			}
			if(inLine.startsWith("height"))
			{
				String[] tokens = inLine.split(" ");
				int height = factor* Integer.parseInt(tokens[1]);
				writer.println("height "+height);
				inLine = reader.readLine();
				continue;

			}
			if(inLine.startsWith("width"))
			{
				String[] tokens = inLine.split(" ");
				int width = factor* Integer.parseInt(tokens[1]);
				writer.println("width "+width);
				inLine = reader.readLine();
				continue;
			}

			StringBuffer outLine = new StringBuffer("");
			for(int i=0; i<inLine.length(); i++)
			{
				if(inLine.charAt(i) == '\r' || inLine.charAt(i) == '\n')
					continue;
				else
				{
					for(int x =0; x< factor; x++)
						outLine.append(inLine.charAt(i));
				}
			}

			for(int x =0; x< factor; x++)
				writer.println(outLine.toString());
			inLine = reader.readLine();
		}
		reader.close();
		writer.close();
	}


	private void readArgs(String[] args)
	{
		for(int i=0; i<args.length; i++)
		{
			String s = args[i];
			if(s.equals("-map"))
			{
				s = args[++i];
				File f = new File(s);
				if(f.exists() && !f.isDirectory())
				{
					targetFile = s;
				}
				else
				{
					System.err.println("-map requires a valid filename");
					System.exit(22);
				}
			}
			else if(s.equals("-f") || s.equals("-factor"))
			{
				try
				{
					s  = args[++i];
					factor = Integer.parseInt(s);
					if(factor <=1 )
					{
						System.err.println("-factor requires an integer argument > 1");
						System.exit(22);
					}
					
				}
				catch(NumberFormatException e)
				{
					System.err.println("-factor requires an integer argument > 1");
				}
			}
			else
			{
					System.err.println("Invalid argument: "+s);
					printHelp();
					System.exit(22);
			}
		}
	}
}



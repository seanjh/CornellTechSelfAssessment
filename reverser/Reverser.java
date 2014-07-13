import java.io.FileReader;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.File;
import java.util.LinkedList;

public class Reverser
{
    public static void main(String[] args) throws IOException
    {
        String filename = parseArgFilename(args);
        String mode = parseArgMode(args);
        //System.out.println("Filename: " + filename);
        //System.out.println("Mode: " + mode);

        FileReader in = null;
        try
        {
            in = new FileReader(filename);
        }
        catch (IOException ex)
        {
            System.err.println("File not found: " + ex.getMessage());
        }

        switch (mode)
        {
            case ("--c"):
                reverseCharacters(in);
                break;
            case ("--w"):
                break;
            case ("--wc"):
                break;
            default:
                throw new UnsupportedOperationException("");
        }

        System.out.println();

        if (in != null)
                in.close();
    }

    private static String parseArgFilename(String[] args)
    {
        String filename = null;

        try
        {
            filename = args[0];
            if (!(new File(filename).isFile())) {
                System.err.printf("ERROR! The file \"%s\" does not exist.\n", filename);
                System.exit(1);
            }
        }
        catch (ArrayIndexOutOfBoundsException ex)
        {
            System.err.println("ERROR! No file name provided.");
            System.exit(1);
        }
        return filename;
    }

    private static String parseArgMode(String[]  args)
    {
        String mode = null;
        try
        {
            mode = args[1];
            if (mode.equals("--c") || mode.equals("--w") || mode.equals("--wc"))
            { }
            else {
                System.err.printf("ERROR! \"%s\" is an unrecognized mode.\n", mode);
                System.exit(1);
            }
        }
        catch (ArrayIndexOutOfBoundsException ex)
        {
            System.err.println("ERROR! No mode provided.");
            System.exit(1);
        }

        return mode;
    }

    private static void reverseCharacters(FileReader in)
    {
        LinkedList<Integer> chars = new LinkedList<Integer>();
        int current;

        try
        {
            current = in.read();
            while (current != -1)
            {
                chars.push(current);
                current = in.read();
            }
        }
        catch (IOException ex)
        {
            System.err.printf("ERROR! Cannot read from stream. %s\n", ex.getMessage());
            System.exit(1);
        }

        while (!chars.isEmpty())
        {
            try
            {
                System.out.printf("%c", chars.pop());
            }
            catch (java.util.IllegalFormatCodePointException ex)
            { }
        }
    }
}

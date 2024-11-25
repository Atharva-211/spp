//package Functions;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class A3 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int code_area_address;
        System.out.print("Enter the code area address: ");
        code_area_address = input.nextInt();
        input.nextLine();  // Consume the newline character

        // Printing the symbol table
        System.out.println("\nSymbol Table:");
        System.out.println("Index | Name | Address");
        int[] index = {1, 2, 3, 4};
        String[] name = {"w", "x", "y", "z"};
        int[] address = {520, 523, 528, 540};
        for (int i = 0; i < name.length; i++) {
            System.out.println(index[i] + " | " + name[i] + " | " + address[i]);
        }

        // Read intermediate code from a file
        File file = new File("intermediate.txt");  // Change the file path if necessary

        try (Scanner fileScanner = new Scanner(file)) {
            String code;
            String[] check;
            int lc = 0, lc_change;

            System.out.println("\nProcessing the intermediate code from the file: ");

            while (fileScanner.hasNextLine()) {
                code = fileScanner.nextLine().trim();  // Read and trim each line
                check = mcode(code);

                // Check if check[2] is a valid number before parsing
                lc_change = check[2].isEmpty() ? 0 : Integer.parseInt(check[2]);

                // Print the code area address and LC
                System.out.print("Code Area Address: " + code_area_address + ", LC: " + lc + " ");

                if (lc_change != 1) {
                    code_area_address += Integer.parseInt(check[1]);
                }

                lc += Integer.parseInt(check[1]);

                if (!check[0].isEmpty()) {
                    // Print the opcode and address
                    System.out.println("Opcode: " + check[0] + ", Address: " + code_area_address);
                } else {
                    // Handle cases where no machine code is generated
                    System.out.println("No machine code generated.");
                }

                System.out.print("\n");
            }

        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        }

        input.close();
    }

    static String[] mcode(String code) {
        String mcode = "", s_operand = "";
        String mnemonic = "", new_size = "", constant = "";
        String[] s1 = new String[3];
        int i = 0, s_address, lc_change = 0;

        // Extract mnemonic
        for (i = 0; i < Math.min(2, code.length()); i++) {
            mnemonic += code.charAt(i);
        }
        mnemonic = mnemonic.toUpperCase();

        switch (mnemonic) {
            case "AD":
                mcode = "";
                i = 5; // Adjust index for consistent processing
                new_size = "0";
                if (i < code.length()) {
                    lc_change = 1;
                    i = 8;
                    while (i < code.length()) {
                        new_size += code.charAt(i);
                        i++;
                    }
                }
                break;

            case "DL":
                String index = "";
                i = 4; // Adjust index to parse properly
                if (i < code.length()) {
                    index += code.charAt(i);
                    i = 8;
                    while (i < code.length()) {
                        constant += code.charAt(i);
                        i++;
                    }
                    if (index.equals("2")) {
                        new_size = "1";
                        mcode += constant;
                    } else {
                       
                    }
                }
                break;

            case "IS":
                new_size = "2";
                i = 3;
                if (i < code.length()) {
                    mcode += code.charAt(i); 
                    i++;
                    if (i < code.length()) {
                        mcode += code.charAt(i);  
                        i += 2;
                        while (i < code.length()) {
                            if (code.charAt(i) == 'R' || code.charAt(i) == 'r') {
                                i += 2;
                                if (i < code.length()) {
                                    mcode += " " + code.charAt(i);
                                }
                            }
                            if (code.charAt(i) == 'S' || code.charAt(i) == 's') {
                                i += 2;
                                if (i < code.length()) {
                                    s_operand = String.valueOf(code.charAt(i));
                                    s_address = Symbol_Table(Integer.parseInt(s_operand));
                                    mcode += " " + s_address;
                                }
                            }
                            i++;
                        }
                    }
                }
                break;

            default:
                System.out.println("Invalid mnemonic: " + mnemonic);
                lc_change = 0; 
                break;
        }

        s1[0] = mcode;
        s1[1] = new_size.isEmpty() ? "0" : new_size;
        s1[2] = String.valueOf(lc_change);
        return s1;
    }

    static int Symbol_Table(int index1) {
        int[] address = {520, 523, 528, 540};
        return (index1 > 0 && index1 <= address.length) ? address[index1 - 1] : -1;
    }
}

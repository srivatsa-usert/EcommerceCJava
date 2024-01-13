/**********************************************************************
    * Title: Main (java)
    * Author: Lakshmi Sai Srivatsa
    * Version: 1.0
    * Last Modified: 13-01-2024
**********************************************************************/

import java.io.IOException;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        int choice;
        Scanner scanner = new Scanner(System.in);

        do {
            System.out.println("\n1. Add Item");
            System.out.println("2. View Items");
            System.out.println("3. View Orders");
            System.out.println("4. Generate Bills");
            System.out.println("0. Exit");
            System.out.print("Enter your choice: ");

            
            choice = scanner.nextInt();

            switch (choice) {
                case 1:
                case 2:
                case 3:
                case 4:
                    runCProgram(choice);
                    break;
                case 0:
                    System.out.println("Exiting the program. Goodbye!");
                    break;
                default:
                    System.out.println("Invalid choice. Please try again.");
            }
        } while (choice != 0);

        scanner.close();
    }

    private static void runCProgram(int choice) {
        String cProgramPath = "ecom.exe";  // Replace with the actual path to your C program
        String command = cProgramPath + " " + choice;

        try {
            Process process = new ProcessBuilder("cmd", "/c", command).inheritIO().start();
            int exitCode = process.waitFor();
            if (exitCode != 0) {
                System.out.println("Error running the C program.");
            }
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}

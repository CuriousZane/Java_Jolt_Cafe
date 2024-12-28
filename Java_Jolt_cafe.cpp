#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Employee
{
protected:
    int employee_ID;
    string employee_name;
    string employee_position;
    string employee_type;
    string start_date;
    double basic_salary;
    double net_salary;

public:
    Employee(int id = 0, string name = "", string position = "", string type = "", string date = "", double salary = 0)
        : employee_ID(id), employee_name(name), employee_position(position), employee_type(type), start_date(date), basic_salary(salary), net_salary(0) {}

    virtual ~Employee() {}

    int getEmployeeID() { return employee_ID; }
    string getPosition() { return employee_position; }
    string getEmployeeType() { return employee_type; }
    string getStartDate() { return start_date; }
    double getBasicSalary() { return basic_salary; }
    double getNetSalary() { return net_salary; }
    virtual double getOvertimeFee() { return 0; }
    virtual int getLeaveDays() { return 0; }
    virtual double getOvertimeHours() { return 0; }

    virtual void calculateSalary() = 0;

    virtual void displayData()
    {
        cout << left << setw(10) << employee_ID
             << setw(20) << employee_name
             << setw(15) << employee_position
             << setw(15) << employee_type
             << setw(15) << start_date
             << setw(12) << fixed << setprecision(2) << net_salary;
    }

    virtual void updateData()
    {
        cout << "Updating Employee Data for ID: " << employee_ID << endl;
        cout << "Enter new name: ";
        cin.ignore();
        getline(cin, employee_name);
        cout << "Enter new position: ";
        getline(cin, employee_position);
        cout << "Enter new start date: ";
        cin >> start_date;
    }
};

class FullTime : public Employee
{
private:
    int leave_days;
    double overtime_hours;
    double bonus;
    double overtime_fees;
    double total_amount;
    double tax_salary;

public:
    FullTime() : leave_days(0), overtime_hours(0), bonus(0), overtime_fees(0), total_amount(0), tax_salary(0) {}

    void setFullTimeData()
    {
        cout << "Enter Employee ID: ";
        cin >> employee_ID;
        cin.ignore();
        cout << "Enter Employee Name: ";
        getline(cin, employee_name);
        cout << "Enter Employee Position (Barista, Cashier, Supervisor): ";
        getline(cin, employee_position);

        if (employee_position == "Barista")
            basic_salary = 1920;
        else if (employee_position == "Cashier")
            basic_salary = 1790;
        else if (employee_position == "Supervisor")
            basic_salary = 2400;
        else
        {
            cout << "Invalid position!" << endl;
            return;
        }
        employee_type = "Fulltime";

        cout << "Enter Start Date (dd/mm/yy): ";
        cin >> start_date;

        cout << "Enter number of leave days: ";
        cin >> leave_days;
        cout << "Enter overtime hours: ";
        cin >> overtime_hours;

        calculateSalary();
    }

    void calculateSalary() override
    {
        double SECURITY_TAX = 10.0;

        if (leave_days == 0)
            bonus = 300;
        else if (leave_days == 1)
            bonus = 200;
        else if (leave_days == 2)
            bonus = 100;
        else
            bonus = 0;

        overtime_fees = ((basic_salary / 30) / 8) * overtime_hours * 2;
        total_amount = basic_salary + bonus + overtime_fees;
        tax_salary = total_amount * 0.02;
        net_salary = total_amount - tax_salary - SECURITY_TAX;
    }

    double getOvertimeFee() override { return overtime_fees; }
    int getLeaveDays() override { return leave_days; }
    double getOvertimeHours() override { return overtime_hours; }
};

class PartTime : public Employee
{
private:
    double hourly_wage;
    double working_hours;
    double total_amount;
    double tax_deduction;

public:
    PartTime() : hourly_wage(0), working_hours(0), total_amount(0), tax_deduction(0) {}

    void setPartTimeData()
    {
        cout << "Enter Employee ID: ";
        cin >> employee_ID;
        cin.ignore();
        cout << "Enter Employee Name: ";
        getline(cin, employee_name);
        cout << "Enter Position (Cleaner, Kitchen Staff, Waiter/Waitress): ";
        getline(cin, employee_position);

        if (employee_position == "Cleaner")
            basic_salary = 1400;
        else if (employee_position == "Kitchen Staff")
            basic_salary = 1850;
        else if (employee_position == "Waiter" || employee_position == "Waitress")
            basic_salary = 1700;
        else
        {
            cout << "Invalid position!" << endl;
            return;
        }
        employee_type = "Parttime";

        cout << "Enter Start Date (dd/mm/yy): ";
        cin >> start_date;

        cout << "Enter working hours (Monthly): ";
        cin >> working_hours;

        calculateSalary();
    }

    void calculateSalary() override
    {
        double SECURITY_TAX = 10.0;

        hourly_wage = basic_salary / (30 * 8);
        total_amount = hourly_wage * working_hours;
        tax_deduction = total_amount * 0.02;
        net_salary = total_amount - tax_deduction - SECURITY_TAX;
    }
};

vector<Employee *> allEmployees;

void insertEmployeeData()
{
    int n;
    cout << "Enter the number of employees to insert: ";
    cin >> n;

    for (int i = 0; i < n; ++i)
    {
        string type;
        cout << "Enter employee type (fulltime/parttime): ";
        cin >> type;

        if (type == "fulltime")
        {
            FullTime *employee = new FullTime();
            employee->setFullTimeData();
            allEmployees.push_back(employee);
        }
        else if (type == "parttime")
        {
            PartTime *employee = new PartTime();
            employee->setPartTimeData();
            allEmployees.push_back(employee);
        }
        else
        {
            cout << "Invalid type! Skipping this employee." << endl;
        }
    }
}

void updateEmployeeData()
{
    int id;
    cout << "Enter the Employee ID to update: ";
    cin >> id;

    for (auto employee : allEmployees)
    {
        if (employee->getEmployeeID() == id)
        {
            employee->updateData();
            employee->calculateSalary();
            cout << "Employee data updated successfully!" << endl;
            return;
        }
    }
    cout << "Employee with ID " << id << " not found!" << endl;
}

void viewFullTimeSalaries()
{
    cout << "\nFull-Time Employees' Salaries:\n";
    cout << "=======================================================================================================================================\n";
    cout << left << setw(10) << "ID" 
         << setw(17) << "Name" 
         << setw(15) << "Position" 
         << setw(15) << "Type" 
         << setw(15) << "Start Date" 
         << setw(15) << "NetSalary" 
         << setw(15) << "LeaveDays" 
         << setw(15) << "OvertimeHrs" 
         << setw(15) << "OvertimeFee" 
         << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";

    for (auto employee : allEmployees)
    {
        if (employee->getEmployeeType() == "Fulltime")
        {
            employee->displayData();
            cout << setw(15) << employee->getLeaveDays()
                 << setw(15) << employee->getOvertimeHours()
                 << setw(15) << employee->getOvertimeFee() 
                 << endl;
        }
    }
}

void viewPartTimeSalaries()
{
    cout << "\nPart-Time Employees' Salaries:\n";
    cout << "==================================================================================================\n";
    cout << left << setw(10) << "ID" 
         << setw(17) << "Name" 
         << setw(15) << "Position" 
         << setw(15) << "Type" 
         << setw(15) << "Start Date" 
         << setw(15) << "NetSalary" 
         << endl;
    cout << "--------------------------------------------------------------------------------------------------\n";

    for (auto employee : allEmployees)
    {
        if (employee->getEmployeeType() == "Parttime")
        {
            employee->displayData();
            cout << endl;
        }
    }
}

void viewAllSalaries()
{
    cout << "\nAll Employees' Salaries:\n";
    cout << "=======================================================================================================================================\n";
    cout << left << setw(10) << "ID" 
         << setw(17) << "Name" 
         << setw(15) << "Position" 
         << setw(15) << "Type" 
         << setw(15) << "Start Date" 
         << setw(15) << "NetSalary" 
         << setw(15) << "LeaveDays" 
         << setw(15) << "OvertimeHrs" 
         << setw(15) << "OvertimeFee" 
         << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";

    if (allEmployees.empty())
    {
        cout << "No employees available!" << endl;
        return;
    }

    int totalEmployees = 0;
    double totalNetSalaries = 0.0;
    double totalOvertimeFees = 0.0;

    for (auto employee : allEmployees)
    {
        employee->displayData();
        cout << setw(15) << employee->getLeaveDays()
             << setw(15) << employee->getOvertimeHours()
             << setw(15) << employee->getOvertimeFee() 
             << endl;

        totalEmployees++;
        totalNetSalaries += employee->getNetSalary();
        totalOvertimeFees += employee->getOvertimeFee();
    }

    cout << "\nMonthly Report Summary:\n";
    cout << "=======================================================================================================================================\n";
    cout << left << setw(25) << "Total Employees" << ": " << totalEmployees << endl;
    cout << left << setw(25) << "Total Net Salaries Paid" << ": $" << fixed << setprecision(2) << totalNetSalaries << endl;
    cout << left << setw(25) << "Total Overtime Fees Paid" << ": $" << fixed << setprecision(2) << totalOvertimeFees << endl;
}


void cleanup()
{
    for (auto employee : allEmployees)
    {
        delete employee;
    }
    allEmployees.clear();
}

int main()
{
    int choice;
    do
    {
        cout << "\nMenu" << endl;
        cout << "1. Insert Employee Data \n"
             << "2. Update Employee Data \n"
             << "3. View Fulltime Employees Salary \n"
             << "4. View Parttime Employees Salary \n"
             << "5. View All Employees Salary and Monthly Report \n"
             << "6. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            insertEmployeeData();
            break;
        case 2:
            updateEmployeeData();
            break;
        case 3:
        viewFullTimeSalaries();
    break;
        case 4:
            viewPartTimeSalaries();
            break;
        case 5:
    // Insertion Sort by Employee ID
    for (size_t i = 1; i < allEmployees.size(); ++i)
    {
        Employee *key = allEmployees[i];
        int j = i - 1;

        while (j >= 0 && allEmployees[j]->getEmployeeID() > key->getEmployeeID())
        {
            allEmployees[j + 1] = allEmployees[j];
            j--;
        }
        allEmployees[j + 1] = key;
    }
    cout << "Employees sorted by ID successfully!" << endl;

    // Display all employees' salaries and monthly report
    viewAllSalaries();
    break;

        case 6:
            cleanup();
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 6);

    return 0;
}

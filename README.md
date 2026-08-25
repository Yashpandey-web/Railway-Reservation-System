# 🚆 Railway Reservation System in C

A menu-driven Railway Reservation System developed in C that simulates the core operations of a railway ticket reservation platform.

The project focuses on train management, passenger management, ticket booking, PNR generation, seat allocation, ticket cancellation, file handling, authentication, and input validation.

---

## ✨ Features

### 🚆 Train Management
- Add new trains
- View available trains
- Search trains
- Update train information
- Delete trains
- Manage available seats
- Store train details persistently

### 👤 Passenger Management
- Add passengers
- View passenger details
- Search passengers
- Update passenger information
- Passenger validation

### 🎫 Ticket Reservation
- Book railway tickets
- Select journey date
- Select travel class
- Automatic seat allocation
- Automatic fare calculation
- PNR generation
- Booking confirmation

### 🔎 PNR Enquiry
- Search tickets using PNR
- View complete ticket information
- Check booking status

### ❌ Ticket Cancellation
- Cancel tickets using PNR
- Update ticket status
- Restore the cancelled seat

### 🔐 Admin System
- Admin authentication
- Train management
- Passenger management
- Reservation management
- System summary

### 🛡️ Input Validation
- Age validation
- Gender validation
- Phone number validation
- Date validation
- Train number validation
- Fare validation
- Invalid input handling

---

## 🛠️ Technologies Used

- **C**
- Structures
- Functions
- Pointers
- Arrays
- Strings
- File Handling
- Binary Files
- `fread()`
- `fwrite()`
- `fseek()`
- `remove()`
- `rename()`

---

## 💾 Data Storage

The system uses binary files for persistent data storage.

The application can create and use files such as:

```text
trains.dat
passengers.dat
reservations.dat
admin.dat

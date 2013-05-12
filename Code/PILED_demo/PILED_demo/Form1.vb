Imports System.IO.Ports
Public Class Form1
    'strobe settings!
    Dim sred, sgreen, sblue As Integer
    Dim sfreq, sduty, sfreqoff As Integer
    Dim strobon As Boolean = False
    Dim ison As Boolean = False
    Dim queue As New ArrayList
    Dim queue_line As Integer = -2

    Dim queueon As Boolean = 0
    Dim ports As String()
    Dim selected_port As String = ""


    Dim com1 As IO.Ports.SerialPort
    Sub init()
        Try
            com1.Close()
        Catch ex As Exception
            'nothing!
        End Try
        Try
            com1 = My.Computer.Ports.OpenSerialPort(selected_port)
            MsgBox("Connected Successfully!")
        Catch ex As Exception
            MsgBox("Failed to Connect - Make sure you have the correct port settings, and try again")
        End Try

    End Sub
    Sub send_output(ByVal data As String)
        ' Send strings to a serial port.
        'MsgBox(data)
        com1.Write(data)
        'com1.Close()
    End Sub
    Private Sub set_rgb(ByVal red As Integer, ByVal green As Integer, ByVal blue As Integer)
        Dim output As String
        output = ("set_rgb(" + Str(red) + "," + Str(green) + "," + Str(blue) + ");")
        output = Replace(output, " ", "")

        send_output(output)
    End Sub
    Private Sub set_rgb_mixer()
        'generate control strings
        set_rgb(Me.NumericUpDown1.Value, Me.NumericUpDown2.Value, Me.NumericUpDown3.Value)
    End Sub

    Private Sub NumericUpDown1_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown1.ValueChanged
        'update the status bar to match the number in the box.
        Me.ProgressBar1.Value = Me.NumericUpDown1.Value
        set_rgb_mixer()


    End Sub

    Private Sub NumericUpDown3_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown3.ValueChanged
        'update the status bar to match the number in the box.
        Me.ProgressBar3.Value = Me.NumericUpDown3.Value
        set_rgb_mixer()

    End Sub

    Private Sub NumericUpDown2_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown2.ValueChanged
        'update the status bar to match the number in the box.
        Me.ProgressBar2.Value = Me.NumericUpDown2.Value
        'generate a new set_rgb string
        set_rgb_mixer()

    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        init()
    End Sub

    Private Sub NumericUpDown4_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown4.ValueChanged
        'Frequency Changed
        sfreq = Me.NumericUpDown4.Value

    End Sub

    Private Sub NumericUpDown8_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown8.ValueChanged
        'red strobe
        sred = Me.NumericUpDown8.Value
    End Sub

    Private Sub NumericUpDown7_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown7.ValueChanged
        'green strobe
        sgreen = NumericUpDown7.Value
    End Sub

    Private Sub NumericUpDown6_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown6.ValueChanged
        'blue strobe
        sblue = NumericUpDown6.Value
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        '
        If strobon = True Then
            strobon = False
            Me.Button2.Text = "Strobe Off"
            Me.Timer1.Enabled = False
        Else
            Me.Button2.Text = "Strobe On"
            Me.Timer1.Enabled = True
            strobon = True
        End If


    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        'Strobe Things! Totally synchronous!
        'turn it on, or off. ALTERNATE IT!
        If ison = True Then
            set_rgb(0, 0, 0)
            Me.Timer1.Interval = sfreqoff
            ison = False
        Else
            set_rgb(sred, sgreen, sblue)
            Me.Timer1.Interval = sfreq
            ison = True
        End If

    End Sub

    Private Sub NumericUpDown5_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumericUpDown5.ValueChanged
        sfreqoff = Me.NumericUpDown5.Value
    End Sub

    Private Sub ListBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs)

    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        Dim temp As String
        Dim builder As String = ""
        temp = ("set_rgb(" + Str(Me.NumericUpDown9.Value) + "," + Str(Me.NumericUpDown10.Value) + "," + Str(Me.NumericUpDown11.Value) + ");")
        queue.Add(temp)
        queue.Add("     " + Str(Me.NumericUpDown12.Value) + Chr(13) + Chr(10))
        For Each line In queue
            builder = builder + line
        Next
        Me.TextBox1.Text = builder
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        queue.Clear()
        Me.TextBox1.Text = ""
        queue_line = -2
    End Sub
    Sub execute_next()
        'Execute the next event in the queue
        'Current line is stored as queue_line
        queue_line += 2
        If queue.ToArray.Length <= queue_line Then
            queue_line = 0
        End If
        If queue.ToArray.Length > 0 Then 'It's not cool to try to read from an empty array. DON'T DO IT
            send_output((queue.ToArray)(queue_line)) ' Set the output as required
            Timer2.Interval = Val((queue.ToArray)(queue_line + 1)) 'set the time delay for the next one
        End If
    End Sub
    Private Sub Timer2_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer2.Tick
        'Timer for queue events. Set the tick time to the duration of an event to enable proper scheduling...
        execute_next()
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        If queueon = True Then
            queueon = False
            Me.Button4.Text = "Click to Start"
            Me.Timer2.Enabled = False
        Else
            Me.Button4.Text = "Click to Stop"
            Me.Timer2.Enabled = True
            queueon = True
        End If
    End Sub

    Private Sub ListBox1_SelectedIndexChanged_1(ByVal sender As System.Object, ByVal e As System.EventArgs)

    End Sub

    Private Sub Form1_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        ports = SerialPort.GetPortNames()
        For Each port As String In ports
            Me.ComboBox1.Items.Add(port)
        Next

        'Ports are enumerated
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged
        'You picked a com port! Congrats!
        selected_port = ports(Me.ComboBox1.SelectedIndex)
    End Sub
End Class

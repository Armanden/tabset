#!/usr/bin/env python3
"""
Guitar Tab Editor - GUI with mouse support
Visual sheet music style fretboard with infinite scrolling
"""

import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import os

class GuitarTabEditor:
    STRINGS = ['e', 'B', 'G', 'D', 'A', 'E']  # High to low
    NUM_STRINGS = 6
    FRET_MARKERS = [3, 5, 7, 9, 12, 15, 17, 19, 21, 22, 24]
    
    def __init__(self, root):
        self.root = root
        self.root.title("Guitar Tab Editor")
        self.root.geometry("950x700")
        self.root.configure(bg='#1a1a2e')
        
        self.tab = {}  # Dict: {(string, fret): value}
        self.selected_string = 0
        self.selected_fret = 0
        self.scroll_x = 0
        self.visible_frets = 24
        
        self._setup_styles()
        self._create_ui()
        self._draw_fretboard()
    
    def _setup_styles(self):
        style = ttk.Style()
        style.theme_use('clam')
        style.configure('Fret.TButton', background='#16213e', foreground='#f39c12')
    
    def _create_ui(self):
        main_frame = tk.Frame(self.root, bg='#1a1a2e')
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        title = tk.Label(main_frame, text="🎸 GUITAR TAB EDITOR", 
                      font=('Helvetica', 20, 'bold'),
                      fg='#f39c12', bg='#1a1a2e')
        title.pack(pady=(0, 10))
        
        tunings_frame = tk.Frame(main_frame, bg='#1a1a2e')
        tunings_frame.pack(side=tk.TOP, pady=(0, 5))
        
        for i, s in enumerate(self.STRINGS):
            color = '#e94560' if i == 0 else '#c4c4c4'
            frm = tk.Frame(tunings_frame, bg='#16213e', bd=1, relief=tk.SUNKEN)
            frm.pack(side=tk.LEFT, padx=2)
            tk.Label(frm, text=s, font=('Consolas', 12, 'bold'), 
                   fg=color, bg='#16213e', width=2).pack(padx=5, pady=2)
        
        self.canvas = tk.Canvas(main_frame, bg='#16213e', height=280,
                           highlightthickness=0, bd=2, relief=tk.SUNKEN)
        self.canvas.pack(fill=tk.X, pady=5)
        
        self.canvas.bind('<MouseWheel>', self._on_mousewheel)
        self.canvas.bind('<Button-4>', self._on_scroll_up)
        self.canvas.bind('<Button-5>', self._on_scroll_down)
        
        self.fretboard_inner = tk.Frame(self.canvas, bg='#16213e')
        self.canvas_window = self.canvas.create_window(0, 0, window=self.fretboard_inner, anchor=tk.NW)
        
        self.h_scroll = tk.Scrollbar(main_frame, orient=tk.HORIZONTAL,
                                  command=self._on_hscroll)
        self.h_scroll.pack(fill=tk.X, pady=(0, 10))
        self.canvas.config(xscrollcommand=self.h_scroll.set)
        
        input_frame = tk.Frame(main_frame, bg='#1a1a2e')
        input_frame.pack(pady=10)
        
        tk.Label(input_frame, text="Fret#:", 
              fg='#ecf0f1', bg='#1a1a2e', font=('Helvetica', 12)).pack(side=tk.LEFT, padx=5)
        
        self.fret_entry = tk.Entry(input_frame, width=4, font=('Helvetica', 14, 'bold'),
                          justify=tk.CENTER, bg='#16213e', fg='#f39c12', insertbackground='#f39c12')
        self.fret_entry.pack(side=tk.LEFT, padx=5)
        self.fret_entry.bind('<Return>', lambda e: self._apply_fret())
        self.fret_entry.bind('<KP_Enter>', lambda e: self._apply_fret())
        
        apply_btn = tk.Button(input_frame, text="Apply", command=self._apply_fret,
                        bg='#e94560', fg='white', font=('Helvetica', 11, 'bold'),
                        width=8, bd=0, padx=10, pady=2)
        apply_btn.pack(side=tk.LEFT, padx=10)
        
        btn_frame = tk.Frame(main_frame, bg='#1a1a2e')
        btn_frame.pack(pady=10)
        
        buttons = [
            ("Clear", self._clear, '#e74c3c'),
            ("Show Tab", self._show_tab, '#3498db'),
            ("Save .tab", self._save_tab, '#2ecc71'),
            ("Save PDF", self._save_pdf, '#9b59b6'),
        ]
        
        for text, cmd, color in buttons:
            btn = tk.Button(btn_frame, text=text, command=cmd,
                          bg=color, fg='white', font=('Helvetica', 10, 'bold'),
                          width=12, bd=0, padx=10, pady=5)
            btn.pack(side=tk.LEFT, padx=5)
        
        self.output_text = tk.Text(main_frame, height=10, width=80,
                            font=('Courier', 10), bg='#16213e', fg='#f39c12',
                            state='disabled')
        
        help_frame = tk.Frame(main_frame, bg='#16213e', bd=1, relief=tk.SUNKEN)
        help_frame.pack(fill=tk.X, pady=(10, 0))
        
        help_text = "Controls: Click fret to select | Enter number + Apply or Enter | ←→ Arrow keys | Mouse wheel to scroll"
        tk.Label(help_frame, text=help_text, fg='#7f8c8d', bg='#16213e',
               font=('Helvetica', 9)).pack(padx=10, pady=5)
        
        self.root.bind('<Up>', lambda e: self._move_selection(-1, 0))
        self.root.bind('<Down>', lambda e: self._move_selection(1, 0))
        self.root.bind('<Left>', lambda e: self._move_selection(0, -1))
        self.root.bind('<Right>', lambda e: self._move_selection(0, 1))
        self.root.bind('<Delete>', lambda e: self._remove_note())
        self.root.bind('<BackSpace>', lambda e: self._remove_note())
        
        self.root.bind('<Home>', lambda e: self._jump_to_start())
        self.root.bind('<End>', lambda e: self._jump_to_end())
        
        self.root.bind('<MouseWheel>', self._on_mousewheel)
    
    def _on_hscroll(self, *args):
        self.canvas.xview(*args)
    
    def _on_scroll_up(self, event=None):
        self._on_mousewheel(event, direction='down')
    
    def _on_scroll_down(self, event=None):
        self._on_mousewheel(event, direction='up')
    
    def _on_mousewheel(self, event, direction=None):
        if direction == 'up' or (hasattr(event, 'delta') and event.delta > 0):
            self.scroll_x = self.scroll_x + 1
        else:
            self.scroll_x = max(0, self.scroll_x - 1)
        
        self._update_canvas_config()
        self._draw_fretboard()
    
    def _update_canvas_config(self):
        total_width = (self.visible_frets + self.scroll_x) * 34
        self.canvas.config(scrollregion=(0, 0, total_width, 280))
    
    def _draw_fretboard(self):
        for widget in self.fretboard_inner.winfo_children():
            widget.destroy()
        
        fret_start = self.scroll_x
        fret_end = fret_start + self.visible_frets
        
        tk.Label(self.fretboard_inner, text="", bg='#16213e', width=2).grid(row=0, column=0, padx=2)
        
        for f in range(fret_start, fret_end):
            color = '#e94560' if f in self.FRET_MARKERS else '#7f8c8d'
            tk.Label(self.fretboard_inner, text=f"{f:2}", 
                  font=('Consolas', 9), fg=color, bg='#16213e',
                  width=4).grid(row=0, column=f-fret_start+1, padx=1)
        
        for s in range(self.NUM_STRINGS):
            color = '#e94560' if s == 0 else '#c4c4c4'
            tk.Label(self.fretboard_inner, text=self.STRINGS[s], 
                  font=('Consolas', 12, 'bold'), fg=color, bg='#16213e',
                  width=2).grid(row=s+1, column=0, padx=2)
            
            tk.Frame(self.fretboard_inner, bg='#e94560', height=22, width=3).grid(
                row=s+1, column=1, padx=(10,2))
            
            for f in range(fret_start, fret_end):
                self._create_fret_button(s, f)
    
    def _create_fret_button(self, s, f):
        col = f - self.scroll_x + 2
        
        is_selected = (s == self.selected_string and f == self.selected_fret)
        val = self.tab.get((s, f), 0)
        
        if val > 0:
            bg = '#e94560'
            fg = 'white'
            text = f"{val}"
        else:
            bg = '#2c3e50' if f in self.FRET_MARKERS else '#1a1a2e'
            fg = '#e94560' if f in self.FRET_MARKERS else '#7f8c8d'
            text = "•" if f in self.FRET_MARKERS else ""
        
        if is_selected:
            bg = '#f39c12'
            fg = 'black'
            if val == 0:
                text = "+"
        
        btn = tk.Button(self.fretboard_inner, text=text, width=3, height=1,
                    font=('Consolas', 11, 'bold'),
                    bg=bg, fg=fg, bd=0, highlightthickness=0,
                    command=lambda ss=s, ff=f: self._select_position(ss, ff))
        btn.grid(row=s+1, column=col, padx=1, pady=1)
        
        if f not in self.FRET_MARKERS:
            tk.Frame(self.fretboard_inner, bg='#e94560', height=22, width=1).grid(
                row=s+1, column=col+1, sticky='ns')
    
    def _select_position(self, s, f):
        self.selected_string = s
        self.selected_fret = f
        self.fret_entry.delete(0, tk.END)
        val = self.tab.get((s, f), 0)
        if val > 0:
            self.fret_entry.insert(0, str(val))
        self._draw_fretboard()
    
    def _move_selection(self, ds, df):
        self.selected_string = max(0, min(self.NUM_STRINGS-1, self.selected_string + ds))
        
        new_fret = self.selected_fret + df
        if new_fret < 0:
            new_fret = 0
            self.scroll_x = 0
        elif new_fret >= self.scroll_x + self.visible_frets:
            self.scroll_x = max(0, new_fret - self.visible_frets + 1)
        elif new_fret < self.scroll_x:
            self.scroll_x = max(0, new_fret)
        
        self.selected_fret = new_fret
        self._update_canvas_config()
        self._draw_fretboard()
        
        self.fret_entry.delete(0, tk.END)
        val = self.tab.get((self.selected_string, self.selected_fret), 0)
        if val > 0:
            self.fret_entry.insert(0, str(val))
    
    def _jump_to_start(self):
        self.scroll_x = 0
        self.selected_fret = 0
        self._draw_fretboard()
    
    def _jump_to_end(self):
        max_fret = max([k[1] for k in self.tab.keys()] + [0])
        self.scroll_x = max(0, max_fret - self.visible_frets + 2)
        self.selected_fret = max_fret
        self._update_canvas_config()
        self._draw_fretboard()
    
    def _apply_fret(self):
        try:
            val = int(self.fret_entry.get())
            if 0 <= val <= 24:
                self.tab[(self.selected_string, self.selected_fret)] = val
                self._draw_fretboard()
                self.fret_entry.delete(0, tk.END)
        except ValueError:
            pass
    
    def _remove_note(self):
        if (self.selected_string, self.selected_fret) in self.tab:
            del self.tab[(self.selected_string, self.selected_fret)]
        self._draw_fretboard()
    
    def _get_max_fret(self):
        if not self.tab:
            return 15
        return max([k[1] for k in self.tab.keys()] + [15])
    
    def _get_ascii(self):
        max_fret = self._get_max_fret()
        lines = []
        for s in range(self.NUM_STRINGS):
            line = self.STRINGS[s] + "-"
            for f in range(max_fret + 1):
                val = self.tab.get((s, f), 0)
                if val > 0:
                    line += f"{val}--"
                else:
                    line += "---"
            lines.append(line)
        return "\n".join(lines)
    
    def _show_tab(self):
        if self.output_text.winfo_ismapped():
            self.output_text.pack_forget()
        else:
            self.output_text.config(state='normal')
            self.output_text.delete('1.0', tk.END)
            self.output_text.insert('1.0', self._get_ascii())
            self.output_text.config(state='disabled')
            self.output_text.pack(pady=5)
    
    def _clear(self):
        self.tab = {}
        self._draw_fretboard()
    
    def _save_tab(self):
        name = filedialog.asksaveasfilename(defaultextension='.tab',
                                     filetypes=[('Tab files', '*.tab'), ('All files', '*')])
        if name:
            with open(name, 'w') as f:
                f.write(self._get_ascii())
            messagebox.showinfo("Saved", f"Saved to {name}")
    
    def _save_pdf(self):
        import subprocess
        
        name = filedialog.asksaveasfilename(defaultextension='.tex',
                                     filetypes=[('LaTeX', '*.tex'), ('All files', '*')])
        if not name:
            return
        
        tex = f"""\\documentclass[letterpaper,12pt]{{article}}
\\usepackage[T1]{{fontenc}}
\\usepackage[utf8]{{inputenc}}
\\usepackage{{geometry}}
\\geometry{{margin=0.75in}}

\\title{{\\textbf{{Guitar Tab}}}}
\\author{{Guitar Tab Editor}}
\\date{{}}

\\begin{{document}}

\\maketitle

\\begin{{verbatim}}
{self._get_ascii()}
\\end{{verbatim}}

\\end{{document}}
"""
        
        with open(name, 'w') as f:
            f.write(tex)
        
        pdf_name = name.replace('.tex', '.pdf')
        
        try:
            result = subprocess.run(['pdflatex', '-interaction=nonstopmode', name],
                            capture_output=True, timeout=30)
            if result.returncode == 0:
                messagebox.showinfo("PDF", f"PDF saved to {pdf_name}")
            else:
                messagebox.showwarning("PDF", "LaTeX failed. Saved .tex")
        except FileNotFoundError:
            messagebox.showwarning("PDF", "pdflatex not found")
        except subprocess.TimeoutExpired:
            messagebox.showwarning("PDF", "Timeout")


def main():
    root = tk.Tk()
    app = GuitarTabEditor(root)
    root.mainloop()


if __name__ == "__main__":
    main()
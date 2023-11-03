class link:
    def __init__(self):
        self.next = link()
        self.data = b''

ctx = link()
ctx = ctx.next.next.next.next.next()

class dRange:
    def __call__(self, start, stop, step):
        r = start
        while r < stop:
            yield r
            r += step

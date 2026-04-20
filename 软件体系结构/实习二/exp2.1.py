
import re
import sys
sys.stdout.reconfigure(encoding='utf-8')

class CalcInterpreter:
    def __init__(self):
        self.tokens = []
        self.current_token = None
        self.token_index = 0
    
    def tokenize(self, text):
        #使用正则表达式匹配数字和运算符
        token_pattern = r'\d+|[+\-*/()]'
        tokens = re.findall(token_pattern, text)
        
        # 验证是否有非法字符
        cleaned_text = re.sub(r'\s+', '', text)
        valid_chars = re.sub(r'\d+|[+\-*/()]', '', cleaned_text)
        if valid_chars:
            return None
        
        return tokens
	#获取下一个token
    def get_next_token(self):
        if self.token_index < len(self.tokens):
            token = self.tokens[self.token_index]
            self.token_index += 1
            return token
        return None
        
    #查看下一个token
    def peek_next_token(self):
        if self.token_index < len(self.tokens):
            return self.tokens[self.token_index]
        return None
    
    #解析表达式
    def parse_expression(self):
		#按运算顺序，优先处理乘除法
        result = self.parse_term()
        
        while self.current_token in ('+', '-'):
            op = self.current_token
            self.current_token = self.get_next_token()
            right = self.parse_term()
            
            if op == '+':
                result += right
            else:
                result -= right
        
        return result
    
    #处理乘除法
    def parse_term(self):
        #按运算顺序，优先处理括号
        result = self.parse_factor()
        
        while self.current_token in ('*', '/'):
            op = self.current_token
            self.current_token = self.get_next_token()
            right = self.parse_factor()
            
            if op == '*':
                result *= right
            else:
                if right == 0:
                    raise ValueError("除零错误")
                result //= right
        
        return result
    
    #处理数字和括号
    def parse_factor(self):
        if self.current_token is None:
            raise ValueError("表达式不完整")
        
        if self.current_token.isdigit():
            result = int(self.current_token)
            self.current_token = self.get_next_token()
            return result
        elif self.current_token == '(':
            self.current_token = self.get_next_token()
            result = self.parse_expression()
            
            if self.current_token != ')':
                raise ValueError("缺少右括号")
            
            self.current_token = self.get_next_token()
            return result
        else:
            raise ValueError(f"意外的token: {self.current_token}")
    
    #计算读入的表达式的值
    def evaluate(self, expression):
        try:
            #去除空格
            expression = expression.strip()
            
            #进行词法分析
            self.tokens = self.tokenize(expression)
            if self.tokens is None:
                return "错误：表达式包含非法字符"
            
            #验证表达式结构是否正确与完整
            if not self.validate_expression():
                return "错误：表达式格式不正确"
            
            # 重置解析状态
            self.token_index = 0
            self.current_token = self.get_next_token()
            
            # 语法分析并计算
            result = self.parse_expression()
            
            # 检查是否还有未处理的token
            if self.current_token is not None:
                return "错误：表达式不完整"
            
            return result
            
        except ValueError as e:
            return f"错误：{str(e)}"
        except Exception as e:
            return f"错误：计算过程中发生异常 - {str(e)}"
    
    #验证表达式的基本结构
    def validate_expression(self):
        if not self.tokens:
            return False
        
        #检查括号匹配
        paren_count = 0
        for i, token in enumerate(self.tokens):
            if token == '(':
                paren_count += 1
            elif token == ')':
                paren_count -= 1
                if paren_count < 0:
                    return False
            
            #检查计算符的使用是否合法
            if i > 0:
                prev = self.tokens[i-1]
                if token in '+-*/' and prev in '+-*/':
                    return False
                if token == '(' and prev.isdigit():
                    return False
                if token.isdigit() and prev == ')':
                    return False
        
        if paren_count != 0:
            return False
        
        
        return True
    #计算器交互界面
    def run(self):
        
        print("简易四则运算计算器")
        print("支持：整数、加减乘除、括号、空格")
        print("输入 'exit'")
        while True:
            expression = input("CALC> ").strip()
            if expression.lower() == 'exit':
                print("再见")
                break
            if not expression:
                continue
            result = self.evaluate(expression)
            print(f"结果: {result}")
			
if __name__ == "__main__":
    # 启动交互式计算器
    interpreter = CalcInterpreter()
    interpreter.run()
